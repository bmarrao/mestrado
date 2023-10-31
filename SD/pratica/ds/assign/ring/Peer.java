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
	public boolean flag;

    public void send(String message)
	{
		this.flag= true;
        this.message = message;
    }

    public String receive() 
	{
		this.flag =false;
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
		Data dataOperations = new Data();
		Peer peer = new Peer(args[0]);
		System.out.printf("new peer @ host=%s\n", args[0]);
		new Thread(new ServerPeer("localhost", Integer.parseInt(args[0]),peer.logger,dataOperations,args[1],Integer.parseInt(args[2]),args[3],Integer.parseInt(args[4]))).start();
		new Thread(new CreateOperations(4,dataOperations)).start();

		if (args.length == 6)
		{
			if (args[5].equals("first"))
			{
				Thread.sleep(25 * 1000);
				synchronized (dataServer) 
				{
					dataServer.send("token");
					dataServer.notifyAll();
				}
			}
		}

    }
}

class CreateOperations implements Runnable {
	PoissonProcess pp;
	private final Data data;
	public CreateOperations (int poissonFrequency, Data data) throws Exception
	{
		this.pp = new PoissonProcess(poissonFrequency, new Random(0));
		this.data=data;

	}

	@Override
	public void run()
	{
		try
		{
			int t;
			Random rand = new Random();
			int op ;
			int x;
			int y;
			String[]operations = {"add","sub","mul","div"};

			while(true)
			{
				try {
					t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
					System.out.println("Irei esperar " + t/1000 + "segundos a thread enquanto dorme");
					Thread.sleep(t);
					System.out.println("Terminou de dormir");
					op = rand.nextInt(4);
					x = rand.nextInt();
					y = rand.nextInt();
					synchronized (data) {
						data.send(operations[op] + ":" + x + ":" + y);
						System.out.println("Coloquei operação");
						data.wait();

					}
				}
				catch(Exception e)
				{
					e.printStackTrace();
				}


			}

		}
		catch(Exception e)
		{
			e.printStackTrace();
		}

	}

}

class ServerPeer implements Runnable {
    String       host;
    int          port;
    ServerSocket server;
    Logger       logger;
	int flag;
	Data dataOperations;
	String Mip;
	String M6ip;
	int Mport;
	int M6port;

    public ServerPeer(String host, int port, Logger logger,Data dataOperations,String Mip , int Mport , String M6ip , int M6port) throws Exception 
	{
		this.host   = host;
		this.port   = port;
		this.logger = logger;
		this.dataOperations= dataOPerations;
		this.Mip = Mip ;
		this.Mport = Mport;
		this.M6ip = M6ip;
		this.M6port = M6port;
    	server = new ServerSocket(port, 1, InetAddress.getByName(host));
    }

    @Override
    public void run()
    {
	try
    {
	    logger.info("server: endpoint running at port " + port + " ...");
		boolean flag;
	    while(true) {
		try {

		    Socket client = server.accept();
		    //logger.info("server: new connection from " + clientAddress);
            try
            {
                BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
                String command;
				command = in.readLine();
                //logger.info("server: message from host " + clientAddress + "[command = " + command + "]");
				try
				{
					synchronized (dataOperations)
					{
						if(dataOperations.flag)
						{
							operation = dataOperations.receive();
							dataOperations.notifyAll();
							flag = true;
						}
						else
						{
							flag = false;
						}
						if (flag)
						{
							socket  = new Socket(InetAddress.getByName(M6ip), M6port);
							logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

							out = new PrintWriter(socket.getOutputStream(), true);
							in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

							out.println(operation);
							out.flush();
							result = in.readLine();

							System.out.printf("result is: %f\n", Double.parseDouble(result));
							socket.close();


						}
						socket  = new Socket(InetAddress.getByName(Mip), Mport);
						logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");
						out = new PrintWriter(socket.getOutputStream(), true);
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
						out.println("token");
						out.flush();
					}

					
				}
				catch(Exception e) 
				{
					e.printStackTrace();
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


