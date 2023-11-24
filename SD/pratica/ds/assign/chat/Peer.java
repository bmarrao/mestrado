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

import ds.assign.poisson.PoissonProcess;

class Tuple
{
    public String ip ;
    public int port;
    public int count;

    public Tuple(String ip, int port)
    {
        this.ip = ip;
        this.port = port;
    }

}

class Data
{
    private ArrayList<String> queue = new ArrayList<String>() ;
    private ArrayList<Tuple> peers;
    int tamanho;
    int counter = 0;
    int meuIndice;
    public Data ( ArrayList<Tuple> peers, int indice)
    {

        this.peers = peers;
        int tamanho = peer.size();
        int meuIndice = indice; 

    }


    public void send(String palavra)
    {
        Socket socket ;
        PrintWriter   out;
        BufferedReader in;
        counter++;
        String message= palavra + ";" + count + ";" + meuIndice;
        for (Tuple tuple : peers)
        {
            socket  = new Socket(InetAddress.getByName(tuple.ip), tuple.port);
            out = new PrintWriter(socket.getOutputStream(), true);
            System.out.println("Array que to mandando " + arr);
            out.println(message);
            out.flush();
            socket.close();

        }
    }

    public void receive(String pack)
    {
        String[] unpack = pack.split(";");
        int ts = Integer.parseInt(unpack[1]);
        int ind =  Integer.parseInt(unpack[2]);
        String palavra = unpack[0];

        if (ts > this.counter)
        {
            this.counter = ts;
        }
        if(!(unpack[0].equals(ack)))
        {
            this.send("ack");
        }
        this.queue.add(unpack[0]+";" + unpack[2]);
        this.deliver();
    }

    public void deliver()
    {
        String m = queue.get(0);
        if(m.equals("ack"))
        {
            queue.remove(0);
        }
        else
        {
            int[] arr = new int[peers.size()];
            String[] unpack =  pack.split(";");;
            int ind;
            for(String pack :queue)
            {
                unpack = pack.split(";");
                ind = Integer.parseInt(unpack[1]);
                arr[ind] = 1;

            }

            for(int i = 0 ; i< arr.length ; i++)
            {
                if (arr[i] != 1)
                {
                    return ;           
                }

            }

            counter++;
            System.out.println(unpack[0]);

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
        Data data = new Data();
        
        for (int i = 1; i < args.length; i+=2)
        {
            peers.add(new Tuple(args[i],Integer.parseInt(args[i+1])));
        }
        
        new Thread (new Server(Integer.parseInt(args[0]),data, peer.logger)).start();
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
    int t ;

    ArrayList<String> dic = new ArrayList<String>() ;

       
    public Client(ArrayList<Tuple> peers,Data data,Logger logger, int frequency)
    {
        this.peers = peers;
        this.logger = logger;
        this.data =data;
        this.pp = new PoissonProcess(frequency, new Random(0));
        this.tamanho = peers.size();
        String r ;
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
            String arr ="";
            Socket socket ;
            PrintWriter   out;
            BufferedReader in;
            int t;
            Random rand =new Random();
            int peer ;
            Tuple tuple;
            inr x ;
            String palavra="";
            synchronized (data) 
            {
                data.append(dic.get(x));
            }
            while(true)
            {
                try 
                {
                    t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                    Thread.sleep(t);
                    x = rand.nextInt(466549);
                    palavra= this.dic.get(x);
                    peer = rand.nextInt(this.tamanho-1) + 1;
                    tuple = this.peers.get(peer);
                    synchronized(data)
                    {
                        data.send(palavra);
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
            BufferedReader in ;
            PrintWriter out;
            int ts ;
            logger.info("server: endpoint running at port " + port + " ...");
            while(true) 
            {
                try 
                {
                    client = server.accept();
                    logger.info("server: new connection from " + client.getInetAddress().getHostAddress());
                    in = new BufferedReader(new InputStreamReader(client.getInputStream()));    
	                out = new PrintWriter(client.getOutputStream(), true);
                    String message = in.readLine();
                    synchronized(data)
                    {
                        data.receive(message);
                    }
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

        
