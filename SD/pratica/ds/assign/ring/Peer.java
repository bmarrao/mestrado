package ds.assing.ring;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;
import java.util.concurrent.locks.Condition;
import java.util.logging.Logger;
import java.util.logging.FileHandler;
import java.util.logging.SimpleFormatter;
import java.util.Random;

import ds.assign.poisson.PoissonProcess;


public class Peer {
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
    
    public static void main(String[] args) throws Exception {
		Peer peer = new Peer(args[0]);
		System.out.printf("new peer @ host=%s\n", args[0]);
        Lock lock = new ReentrantLock();
        Condition tenhoToken = lock.newCondition();
        String[] m = args[1].split(' ');
        String[] m6 = args[2].split(' ');
		new Thread(new Server("localhost", Integer.parseInt(args[1]),tenhoToken)).start();
        new Thread(new Client("localhost",  m[0],Integer.parseInt(m[1]),m6[0],Integer.parseInt(m6[1]),tenhoToken,peer.logger)).start();
		if (args.length == 4)
		{
			if (args[3].equals("first"))
			{
				tenhoToken.notify();
			}
		}

    }
}



class Server implements Runnable {
    String       host;
    int          port;
    ServerSocket server;
    Logger       logger;
    Condition tenhoToken;
	int flag;
    
    public Server(String host, int port, Logger logger, Condition  tenhoToken) throws Exception {
	this.host   = host;
	this.port   = port;
	this.logger = logger;
    this.tenhoToken = tenhoToken;
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
                BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));    
	            PrintWriter out = new PrintWriter(client.getOutputStream(), true);
                String command;
                command = in.readLine();
                logger.info("server: message from host " + clientAddress + "[command = " + command + "]");
				if (command.equals("token"))
				{
					tenhoToken.notify();
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
	Int Mport;
	Int M6port;
	Condition tenhoToken;
    Logger  logger;
    Scanner scanner;
	PoissonProcess pp;
    

	//	new Thread(new Client("localhost",  m[0],Integer.parseInt(m[1]),m6[0],Integer.parseInt(m6[1]),tenhoToken,peer.logger)).start();

    public Client(String host, String Mip , Int Mport , String M6ip , Int M6porto,Condition tenhoToken, Logger logger) throws Exception 
	{
		this.host    = host;
		this.logger  = logger; 
		this.Mip = Mip ;
		this.Mport = Mport;
		this.M6ip = M6ip;
		this.M6ip = M6ip;
		this.tenhoToken= tenhoToken;
		this.logger = logger;
		this.pp = new PoissonProcess(4, new Random(0));

    }

    @Override 
    public void run() 
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
	    while (true) 
		{
		try 
		{
			

			try
			{
				t = (int)(this.pp.timeForNextEvent()*60.0*1000.0);
				Thread.sleep(t);
				tenhoToken.await();
				Socket socket  = new Socket(InetAddress.getByName(M6ip), M6port);
				logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

				PrintWriter   out = new PrintWriter(socket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));    
				op = rand.nextInt(4);
				x= rand.nextInt();
				y = rand.nextInt();
				out.println(operations[op]+' '+x+' '+y+'\n');
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
			catch(Exception e) {
		    e.printStackTrace();
			}   lose();

		} catch(Exception e) {
		    e.printStackTrace();
		}   
	    }
	} catch(Exception e) {
	    e.printStackTrace();
	}   	    
    }
}
