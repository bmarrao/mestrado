package ds.examples.sockets.peer;

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
import java.util.Random;
import java.util.concurrent.locks.*;

import ds.examples.sockets.poisson.PoissonProcess;
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

class Data
{
    private ArrayList<Double> arr = new ArrayList<Double>() ;
    Lock l = new ReentrantLock();


    public void append(double x)
    {

        this.arr.append(x);

    }

    public String getStringArr()
    {
        return this.arr.toString();
    }

    public String give(String arrayPush)
    {

        ArrayList<Double> newArray = new ArrayList<Double>() ;
        ArrayList<Double> received = new ArrayList<Double>() ;
        //Transform arrayPush in received
        int maior ;
        double novo ;
        double recebido;
        if (received.length > this.arr.length)
        {
            maior = received.length;
        }
        else
        {
            maior = this.arr.length;
        }
        for (int i = 0; i< maior ; i++)
        {
            novo = this.arr[i];
            recebido = received[i];
            if (novo && recebido)
            {

                newArray.append(newArray[i])
            }
            else if ()
            {
                append(i);
            }
        }

        this.arr = newArray;
        return newArray.toString();

        
    }

    public void receive(String newArr)
    {
        this.arr = newArr;
    }

}
public class Peer {
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
        for (int i = 2 ; i < args.length ; i+= 2)
        {
            peers.append(Tuple(args[i],args[i+1]));
        }
        //Array peers for when there is more than one connection
        new Thread (new Server(args[0], Integer.parseInt(args[1]),data, peer.logger)).start();
        //CLient para pedir pros outros push and pull
        new Thread (new Client(args[2],Integer.parseInt(args[3],1),data,peer.logger)).start();
        new Thread (new PutInArray(4,data)).start();
    }
}

class Client implements Runnable 
{
    String       host;
    int          port;
    Logger       logger;
    Data data;
    PoissonProcess pp;

    public Client(String host, int port, Data data,Logger logger, int frequency)
    {
        this.host   = host;
        this.port   = port;
        this.logger = logger;
        this.data =data;
        this.ṕp = PoissonProcess(frequency, new Random(0));

    }

    public void run()
    {
        try
        {
            String arr ="";
            Socket socket ;
            PrintWriter   out;
            BufferedReader in;
            while(true)
            {
                try 
                {
                    t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                    Thread.sleep(t);
                    
                    synchronized(data)
                    {
                        arr = data.getStringArr();
                        socket  = new Socket(InetAddress.getByName(host), port);
						out = new PrintWriter(socket.getOutputStream(), true);
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        out.println(arr);
                        out.flush();
                        arr = in.readLine();
                        data.receive(arr);
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
    String       host;
    int          port;
    ServerSocket server;
    Logger       logger;
    Data data;
    

    public Server(String host, int port, Data data,Logger logger) throws Exception 
    {
        this.host   = host;
        this.port   = port;
        this.logger = logger;
        this.data =data;
        server = new ServerSocket(port, 1, InetAddress.getByName(host));
    }

    @Override
    public void run() 
    {
        try 
        {
            Socket client ;
            String clientAdress ="";
            String arr ="";
            String result ="";
            PrintWriter   out;
            BufferedReader in ;
            logger.info("server: endpoint running at port " + port + " ...");
            while(true) {
                try {
                    client = server.accept();
                    clientAddress = client.getInetAddress().getHostAddress();
                    logger.info("server: new connection from " + clientAddress);
                    out = new PrintWriter(socket.getOutputStream(), true);
                    in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    arr = in.readLine();
                    synchronized(data)
                    {
                        result = data.give(arr);
                    }
                    out.println(result);
                    out.flush();
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


class PutInArray implements Runnable 
{
    String  host;
    Logger  logger;
    Scanner scanner;
    PoissonProcess pp;
    private final Data data;
    public PutInArray(int frequency, Data data) throws Exception {
        this.data = data;
        this.pp = new PoissonProcess(frequency, new Random(0));

    }

    @Override
    public void run() {
        int t ;
        Random rand =new Random();
        int x ;
        while(true)
        {
            try
            {
                t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                Thread.sleep(t);
                synchronized (data) 
                {
                    data.append(rand.nextDouble());
                }
            }
            catch(Exception e)
            {
                e.printStackTrace();
            }
        }
           
    }
    
}
