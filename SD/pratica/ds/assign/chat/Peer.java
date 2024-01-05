package ds.assign.chat;

import java.io.*;  
import java.util.Scanner;  
import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.logging.Logger;
import java.util.logging.FileHandler;
import java.util.logging.SimpleFormatter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.locks.*;
import java.lang.*;
import java.util.Comparator;

import ds.assign.poisson.PoissonProcess;
//Classe de tuplos para guardar ip e port
class Tuple
{
    public String ip ;
    public int port;

    public Tuple(String ip, int port)
    {
        this.ip = ip;
        this.port = port;
    }
}

//Classe para guardar atributos de mensagens
class Message
{
    public String payload ;
    public int cj;
    public int indice;

    public Message(String payload, int cj,int indice)
    {
        this.payload = payload;
        this.cj = cj;
        this.indice = indice;
    }
} 

//Classe que manda mensagens , recebe mensagens e entrega mensagens para o processo
class Data
{
    // Mensagens que estão na fila
    private ArrayList<Message> queue = new ArrayList<Message>() ;
    // Peers que estão conectados
    private ArrayList<Tuple> peers;
    int tamanho;
    int counter = 0;
    int meuIndice;

    //Função de comparação para dar sort no array , que tem como segunda opção caso tenha o mesmo clock ordenar por indice
    Comparator<Message> comparator = new Comparator<>() {
        @Override
        public int compare(Message m1, Message m2) 
        {
            int comp1 = m1.cj-m2.cj;

            if (comp1!= 0) {
               return comp1;
            } 
            return m1.indice-m2.indice;
        }
    };
    

    //Inicialização da classe
    public Data ( ArrayList<Tuple> peers, int indice)
    {

        this.peers = peers;
        this.tamanho = peers.size();
        this.meuIndice = indice; 

    }

    // Manda para todos os processos menos si proprio, com o counter , e o indice a mensagem
    public void send(String palavra)
    {
        Socket socket ;
        PrintWriter   out;
        synchronized(this)
        {
            counter++;
            this.queue.add(new Message(palavra,this.counter,this.meuIndice));
        }

        String message= palavra + ";" + this.counter + ";" + this.meuIndice;
        try 
        {
            Tuple tuple ;   

            for (int i = 0 ; i < this.tamanho; i++)
            {

                if (i!= this.meuIndice)
                {
                    tuple= peers.get(i);
                    socket  = new Socket(InetAddress.getByName(tuple.ip), tuple.port);
                    out = new PrintWriter(socket.getOutputStream(), true);
                    out.println(message);
                    out.flush();
                    socket.close();
                }
            }

        }
        catch(Exception e)
        {
            e.printStackTrace();
        }

    }

    // Manda acks para todos os processos 
    public void sendAck(String palavra)
    {
        Socket socket ;
        PrintWriter   out;
        String message= palavra + ";" + this.counter + ";" + this.meuIndice;
        try 
        {
            Tuple tuple ;   

            for (int i = 0 ; i < this.tamanho; i++)
            {
                tuple= peers.get(i);
                socket  = new Socket(InetAddress.getByName(tuple.ip), tuple.port);
                out = new PrintWriter(socket.getOutputStream(), true);
                out.println(message);
                out.flush();
                socket.close();
                
            }

        }
        catch(Exception e)
        {
            e.printStackTrace();
        }

    }

    // Recebe uma nova mensagem atualiza o counter e chama a função deliver
    public void receive(String pack)
    {
        String[] unpack = pack.split(";");
        int ts = Integer.parseInt(unpack[1]);
        int ind =  Integer.parseInt(unpack[2]);
        String palavra = unpack[0];
        int i = 0;
        synchronized(this)
        {
            if(!(unpack[0].equals("ack")))
            {
                i =1;
            }
            if (ts > this.counter)
            {
                this.counter = ts;
            }
            this.queue.add(new Message(palavra,ts,ind));
            this.deliver();

        }

        if(i == 1   )
        {
            this.sendAck("ack");
        }
        

    }


    //Função que caso a mensagem cumpra os requisitos entrega pro processo e caso contrario não faz nada
    public void deliver()
    {
        // Da sort no array para poder pegar a mensagem com menor ack
        this.queue.sort(this.comparator);

        Message m = queue.get(0);

        // Se for ack descarta 
        if(m.payload.split(" ")[0].equals("ack"))
        {
            queue.remove(0);
        }
        //©aso contrario checa os requisitos
        else
        {
            int[] arr = new int[this.tamanho];
            for (int a = 0 ; a < this.tamanho; a++)
            {
                arr[a] = 0;
            }
            arr[this.meuIndice] = 1;


            for(Message messa :queue)
            {
                if (m.cj < messa.cj)
                {
                    arr[messa.indice] = 1;
                }
            }

            int i  = 0;
            for(; i< this.tamanho ; i++)
            {
                if (arr[i] != 1)
                {
                    break;         
                }

            }

            // se cumpre os requisitos imprime
            if(i == this.tamanho)
            {
                System.out.println(m.payload + " " + m.cj );
                queue.remove(0);
                counter++;

            }

        }        
    }

    

}
public class Peer 
{
    String host;
    Logger logger;

    public Peer(String hostname)
    {
        host   = hostname;
        logger = Logger.getLogger("logfile");
        try
        {
            FileHandler handler = new FileHandler("./" + hostname + "_peer.log", true);
            logger.addHandler(handler);
            SimpleFormatter formatter = new SimpleFormatter();
            handler.setFormatter(formatter);
        }
        catch ( Exception e )
        {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws Exception
    {
        Peer peer = new Peer(args[0]);
        System.out.printf("new peer @ host=%s\n", args[0]);
        ArrayList<Tuple> peers = new ArrayList<Tuple>();
        //Coloca os peers no array peers
        for (int i = 0; i < args.length-1; i+=2)
        {
            peers.add(new Tuple(args[i],Integer.parseInt(args[i+1])));
        }
        int indice= Integer.parseInt(args[args.length-1]);

        Data data = new Data(peers,indice);
        
        Tuple meuPeer = peers.get(indice);
        new Thread (new Server(meuPeer.port,data, peer.logger)).start();
        Thread.sleep(30000);

        new Thread (new Client(peers,data,peer.logger,60)).start();
    }
}

class Client implements Runnable 
{
    String       host;
    int          port;
    Logger       logger;
    Data data;
    PoissonProcess pp;
    ArrayList<Tuple> peers;
    int tamanho;
    int t ;

    ArrayList<String> dic = new ArrayList<String>() ;

       
    public Client(ArrayList<Tuple> peers,Data data,Logger logger, int frequency)
    {
        this.peers = peers;
        this.logger = logger;
        this.data =data;
        // Inicializando o poisson process
        this.pp = new PoissonProcess(frequency, new Random(0));
        this.tamanho = peers.size();
        String r ;
        //Aqui iremos inicializar o dicionario para gerarmos palavras aleatorios
        try (FileInputStream fis = new FileInputStream("./ds/assign/entropy/words.txt");
        InputStreamReader isr = new InputStreamReader(fis);
        BufferedReader br = new BufferedReader(isr))
        {
            while((r=br.readLine())!= null)
            {
                this.dic.add(r);
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }


    }

    public void run()
    {
        try
        {
            int t;
            Random rand =new Random();
            int x ;
            String palavra="";
            while(true)
            {
                try 
                {
                    t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                    Thread.sleep(t);
                    //Gerar um numero aleatorio do tamanho do dicionario
                    x = rand.nextInt(466549);
                    palavra= this.dic.get(x);
                    // Enviar palavra generada
                    data.send(palavra);

                }
                catch (Exception e) 
                {
                    e.printStackTrace();
                }
                
            }

        }
        catch (Exception e) 
        {
            e.printStackTrace();
        }
    }
}
class Server implements Runnable 
{
    int          port;
    ServerSocket server;
    Logger       logger;
    Data data;
    ArrayList<String> queue = new ArrayList<String>() ;
    

    public Server(int port, Data data,Logger logger) throws Exception 
    {
        this.port   = port;
        this.logger = logger;
        this.data =data;
        this.server = new ServerSocket(port, 1, InetAddress.getByName("localhost"));
    }

    @Override
    public void run() 
    {
        try 
        {
            Socket client ;
            //logger.info("server: endpoint running at port " + port + " ...");
            while(true) 
            {
                try 
                {
                    client = server.accept();
                    new Thread(new Connection(client, data)).start();
                }
                catch(Exception e) 
                {
                    e.printStackTrace();
                }
            }
        } 
        catch (Exception e) 
        {
            e.printStackTrace();
        }
    }
}
        
class Connection implements Runnable {
    Socket clientSocket;
    private final Data data;

    public Connection(Socket clientSocket,Data data) 
    {
        this.clientSocket  = clientSocket;
        this.data = data;
    }

    @Override
    public void run() 
    {
        try 
        {
            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));    
            String message = in.readLine();
            //Recebe mensagem
            data.receive(message);
            clientSocket.close();

        } 
        catch(Exception e) 
        {
            e.printStackTrace();
        }
    }
}