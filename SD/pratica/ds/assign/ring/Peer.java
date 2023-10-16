package ds.assign.ring;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import java.util.logging.Logger;
import java.util.logging.FileHandler;
import java.util.logging.SimpleFormatter;
import java.util.ArrayList;
import java.util.Random;

import ds.assign.poisson.PoissonProcess;

class Data 
{
    private String message;

    public void send(String message) {
        this.message = message;
    }

    public String receive() {
        return message;
    }
}

public class Peer 
{
    String host;
    Logger logger;
    public Peer(String hostname) {
	host   = hostname;
	logger = Logger.getLogger("logfile");
	try {
	    FileHandler handler = new FileHandler("./" + hostname + "_peer.log", true);
	    logger.addHandler(handler);
	    SimpleFormatter formatter = new SimpleFormatter();
	    handler.setFormatter(formatter);
	} catch ( Exception e ) {
	     e.printStackTrace();
	}
    }

    public static void main(String[] args) throws Exception
	{
		Data data = new Data();
		Peer peer = new Peer(args[0]);
		System.out.printf("new peer @ host=%s\n", args[0]);
		new Thread(new ServerPeer("localhost", Integer.parseInt(args[0]),peer.logger,data)).start();
		System.out.println(args[1] + args);
        new Thread(new Client("localhost", args[1],Integer.parseInt(args[2]),args[3],Integer.parseInt(args[4]),data,peer.logger)).start();
		if (args.length == 4)
		{
			if (args[3].equals("first"))
			{
				Thread.sleep(600 * 1000);
				synchronized (data) 
				{
					data.send("test");
		
					data.notifyAll();
				}
			}
		}

    }
}



class ServerPeer implements Runnable {
    String       host;
    int          port;
    ServerSocket server;
    Logger       logger;
    private final Data data;
	int flag;

    public ServerPeer(String host, int port, Logger logger, Data data) throws Exception {
	this.host   = host;
	this.port   = port;
	this.logger = logger;
    this.data= data;
    server = new ServerSocket(port, 1, InetAddress.getByName(host));
    }

    @Override
    public void run()
    {
	try
    {
	    logger.info("server: endpoint running at port " + port + " ...");
	    while(true) {
		try {
		    Socket client = server.accept();
		    String clientAddress = client.getInetAddress().getHostAddress();
		    logger.info("server: new connection from " + clientAddress);
            try
            {
                BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
	            PrintWriter out = new PrintWriter(client.getOutputStream(), true);
                String command;
				command = in.readLine();
                logger.info("server: message from host " + clientAddress + "[command = " + command + "]");
				if(command.equals("token"))
				synchronized (data) {
					data.send("test");
		
					data.notifyAll();
				}

            }
            catch(Exception e)
            {
	            e.printStackTrace();
	        }
		}catch(Exception e) {
		    e.printStackTrace();
		}
	    }
	} catch (Exception e) {
	     e.printStackTrace();
	}
    }
}



class Client implements Runnable
{
    String  host;
	String Mip;
	String M6ip;
	int Mport;
	int M6port;
	private final Data data;
    Logger  logger;
    Scanner scanner;
	PoissonProcess pp;



    public Client(String host, String Mip , int Mport , String M6ip , int M6porto,Data data, Logger logger) throws Exception
	{
		this.host    = host;
		this.logger  = logger;
		this.Mip = Mip ;
		this.Mport = Mport;
		this.M6ip = M6ip;
		this.M6ip = M6ip;
		this.data= data;
		this.logger = logger;
		this.pp = new PoissonProcess(4, new Random(0));

    }

    @Override
    public  void run()
	{
	try
	{
	    logger.info("client: endpoint running ...\n");

		int t;
		Random rand = new Random();
		int op ;
		int x;
		int y;
		String[]operations = {"add","sub","mul","div"};
		String message ="";
	    while (true)
		{
		try
		{


			try
			{
				t = (int)(this.pp.timeForNextEvent()*60.0*1000.0);
				Thread.sleep(t);
				op = rand.nextInt(4);
				x= rand.nextInt();
				y = rand.nextInt();
				synchronized (data) 
				{
					try 
					{
						data.wait();
						message = data.receive();
					} 
					catch(Exception e) 
					{
						e.printStackTrace();
					}
				}
				if(message.equals("token"))
				{
					Socket socket  = new Socket(InetAddress.getByName(M6ip), M6port);
					logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

					PrintWriter   out = new PrintWriter(socket.getOutputStream(), true);
					BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));    
					
					out.println(operations[op]+" "+x+" "+y+'\n');
					out.flush();	    

					String result = in.readLine();
					System.out.printf(result +"\n");
					socket.close();

					socket  = new Socket(InetAddress.getByName(Mip), Mport);
					out = new PrintWriter(socket.getOutputStream(), true);
					in = new BufferedReader(new InputStreamReader(socket.getInputStream())); 
					out.println("token\n");
					out.flush();
				}
				
			}
			catch(Exception e) {
		    e.printStackTrace();
			}

		} catch(Exception e) {
		    e.printStackTrace();
		}   
	    }
	} catch(Exception e) {
	    e.printStackTrace();
	}   	    
    }
}