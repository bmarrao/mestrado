package ds.assign.entropy;

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

    public Tuple(String ip, int port)
    {
        this.ip = ip;
        this.port = port;
    }


}

class Data
{
    private ArrayList<Double> arr = new ArrayList<Double>() ;


    public String turnToString(ArrayList<Double> xArr)
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

    public ArrayList<Double> turnFromString(String xArr)
    {
        ArrayList<Double> newArr = new ArrayList<Double>() ;

        String[] numbers = xArr.split(",");
        for (String x : numbers)
        {
            newArr.add(Double.parseDouble(x));
        }
        return newArr;
    }

    public void append(double x)
    {
        this.arr.add(x);
    }

    public String getStringArr()
    {
        return this.turnToString(arr);
    }

    
    public String give(String arrayPush)
    {

        ArrayList<Double> newArray = new ArrayList<Double>() ;
        ArrayList<Double> received = new ArrayList<Double>() ;
        received = this.turnFromString(arrayPush);
        int menor ;
        int maior ;
        boolean flag ;
        double novo ;
        double recebido;
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
        for (int i = 0; i< menor ; i++)
        {
            novo = this.arr.get(i);
            recebido = received.get(i);
            if (novo == recebido)
            {
                newArray.add(novo);
            }
            else
            {
                newArray.add(novo);
                newArray.add(recebido);
            }
        }
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
        
        
        //Array peers for when there is more than one connection
        new Thread (new Server(args[0], Integer.parseInt(args[1]),data, peer.logger)).start();
        new Thread (new PutInArray(4,data)).start();
        new Thread (new Client(peers,data,peer.logger,2)).start();
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

    public Client(ArrayList<Tuple> peers,Data data,Logger logger, int frequency)
    {
        this.peers = peers;
        this.logger = logger;
        this.data =data;
        this.pp = new PoissonProcess(frequency, new Random(0));


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
            int tamanho = peers.size();
            Random rand =new Random();
            int peer ;
            Tuple tuple;
            while(true)
            {
                try 
                {
                    t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                    Thread.sleep(t);
                    peer = rand.nextInt(tamanho);
                    tuple = this.peers.get(peer);
                    synchronized(data)
                    {
                        arr = data.getStringArr();
                        socket  = new Socket(InetAddress.getByName(tuple.ip), tuple.port);
						out = new PrintWriter(socket.getOutputStream(), true);
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                        System.out.println("Array que to mandando " + arr);
                        out.println(arr);
                        out.flush();
                        arr = in.readLine();
                        data.receive(arr);

                        System.out.println("array depois "+ data.getStringArr());
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
        double x ;
        while(true)
        {
            try
            {
                t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
                Thread.sleep(t);
                synchronized (data) 
                {
                    x = rand.nextDouble();

                    data.append(x);
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

            synchronized(data)
            {
                String result = data.give(arr);
            }
            System.out.println("saiu do synchronized");
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
