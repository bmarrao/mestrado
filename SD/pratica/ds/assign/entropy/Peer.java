package ds.assign.entropy;

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

import ds.assign.poisson.PoissonProcess;

// Clase pra guardar o ip e port
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

// Faz read and write , serve de comunicação e tem outras funcionalidades para ajudar com a transcrição de array pra string e string pra array
class Data
{
    private ArrayList<String> arr = new ArrayList<String>() ;


    // Transforma array string
    public String turnToString(ArrayList<String> xArr)
    {
        String str = "";
        int tamanho = xArr.size();
        for (int i  = 0; i < tamanho-1; i++)
        {
            str+= xArr.get(i) +"," ;
        }
        str+= xArr.get(tamanho-1);
        return str;

    }

    // Transforma string em arary
    public ArrayList<String> turnFromString(String xArr)
    {
        ArrayList<String> newArr = new ArrayList<String>() ;

        String[] palavras = xArr.split(",");
        for (String x : palavras)
        {
            newArr.add(x);
        }
        return newArr;
    }

    //Adiciona string no array
    public void append(String x)
    {
        
        this.arr.add(x);
    }

    // Transforma array na string
    public String getStringArr()
    {
        return this.turnToString(arr);
    }


    // Faz push and pull
    public String give(String arrayPush)
    {

        ArrayList<String> newArray = new ArrayList<String>() ;
        ArrayList<String> received = new ArrayList<String>() ;
        received = this.turnFromString(arrayPush);
        int menor ;
        boolean flag ;
        String novo ;
        String recebido;
        // Ve qual o menor array 
        if (received.size() < this.arr.size())
        {
            flag = true;
            menor = received.size();
        }
        else
        {
            flag = false;
            menor = this.arr.size();
        }
        // Percorre o menor array até o fina 
        for (int i = 0; i< menor ; i++)
        {
            novo = this.arr.get(i);
            recebido = received.get(i);
            // Se for igual adiciona só uma vez
            if (novo.equals(recebido))
            {
                newArray.add(novo);
            }
            // Se for diferente adiciona os dois
            else
            {
                newArray.add(novo);
                newArray.add(recebido);
            }
        }
        // Adiciona as q sobrarem do menor array
        if (flag)
        {
            for (int j = menor ; j < this.arr.size()  ; j++)
            {
                newArray.add(this.arr.get(j));
            }
        }
        else
        {
            for (int j = menor ; j < received.size()  ; j++)
            {
                newArray.add(received.get(j));
            }
        }
    
        this.arr = newArray;
        return this.turnToString(this.arr);

        
    }

    public void receive(String newArr)
    {
        this.arr = this.turnFromString(newArr);
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
        Data data = new Data();
        
        for (int i = 1; i < args.length; i+=2)
        {
            peers.add(new Tuple(args[i],Integer.parseInt(args[i+1])));
        }
        
        new Thread (new Server(Integer.parseInt(args[0]),data, peer.logger)).start();
        new Thread (new PutInArray(6,data)).start();
        new Thread (new Client(peers,data,peer.logger,1)).start();
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

    public Client(ArrayList<Tuple> peers,Data data,Logger logger, int frequency)
    {
        this.peers = peers;
        this.logger = logger;
        this.data =data;
        this.pp = new PoissonProcess(frequency, new Random(0));
        this.tamanho = peers.size();

    }

    public void run()
    {
        try
        {
            String arr ="";
            Socket socket ;
            PrintWriter   out;
            BufferedReader in;
            int t;
            Random rand =new Random();
            int peer ;
            Tuple tuple;
            while(true)
            {
                try 
                {
                    t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                    Thread.sleep(t);
                    //Seleciona peer aleatoriamente
                    peer = rand.nextInt(this.tamanho);
                    tuple = this.peers.get(peer);
                    synchronized(data)
                    {
                        //Envia o array que temos
                        arr = data.getStringArr();
                        socket  = new Socket(InetAddress.getByName(tuple.ip), tuple.port);
						out = new PrintWriter(socket.getOutputStream(), true);
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        out.println(arr);
                        out.flush();
                        //Recebe o array depois de feita a operação
                        arr = in.readLine();
                        data.receive(arr);
                        //Array no finald do push and pull
                        System.out.println(data.getStringArr());
                    }

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
            logger.info("server: endpoint running at port " + port + " ...");
            while(true) 
            {
                try 
                {
                    client = server.accept();
                    logger.info("server: new connection from " + client.getInetAddress().getHostAddress());
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

//Gera palavras novas pra por no array
class PutInArray implements Runnable 
{
    Scanner scanner;
    PoissonProcess pp;
    private final Data data;
    public PutInArray(int frequency, Data data) throws Exception 
    {
        this.data = data;
        this.pp = new PoissonProcess(frequency, new Random(0));


    }


    @Override
    public void run() {
        int t ;
        Random rand =new Random();
        int x ;
        String r ;
        ArrayList<String> dic = new ArrayList<String>() ;
        //Configura dicionario
        try (FileInputStream fis = new FileInputStream("./ds/assign/entropy/words.txt");
        InputStreamReader isr = new InputStreamReader(fis);
        BufferedReader br = new BufferedReader(isr))
        {
            while((r=br.readLine())!= null)
            {
                dic.add(r);
            }
        }
        catch(Exception e)
        {
            e.printStackTrace();
        }
        while(true)
        {
            try
            {
                t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                Thread.sleep(t);
                //Gera palavra aleatoria do dicionario
                x = rand.nextInt(466549);
                //Adiciona palavra no array
                synchronized (data) 
                {
                    data.append(dic.get(x));
                }
            
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }
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
	        PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            String arr = in.readLine();
            String result ;
            synchronized(data)
            {
                String antes = data.getStringArr();
                result = data.give(arr);
            }
            //Array que temos no final do push and pull
            out.println(result);
            out.flush();
            clientSocket.close();

        } 
        catch(Exception e) 
        {
            e.printStackTrace();
        }
    }
}

