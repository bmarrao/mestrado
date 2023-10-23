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
		Data dataServer = new Data();
		Data dataOperations = new Data();
		Peer peer = new Peer(args[0]);
		System.out.printf("new peer @ host=%s\n", args[0]);
		new Thread(new ServerPeer("localhost", Integer.parseInt(args[0]),peer.logger,data)).start();
        new Thread(new Client("localhost", args[1],Integer.parseInt(args[2]),args[3],Integer.parseInt(args[4]),dataServer,dataOperations,peer.logger)).start();
		new Thread(new createOperations(4,dataOperations)).start();

		if (args.length == 6)
		{
			if (args[5].equals("first"))
			{
				Thread.sleep(25 * 1000);
				synchronized (data) 
				{
					data.send("token");
					data.notifyAll();
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
					System.out.println("Irei esperar thread enquanto dorme");
					t = (int) (this.pp.timeForNextEvent() * 60.0 * 1000.0);
					Thread.sleep(t);
					System.out.println("Terminou de dormir");
					op = rand.nextInt(4);
					x = rand.nextInt();
					y = rand.nextInt();
					System.out.println("ENtrando synchronized createOperation");
					synchronized (data) {
						data.send(operations[op] + ":" + x + ":" + y);
						System.out.println("Esperando o notify createOperation");
						data.wait();
						System.out.println("Já não estou mais esperando createOperation");

					}
				}
				catch(Exception e)
				{
					e.printStackTrace();
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
			System.out.println("New Connection");
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
				{
					System.out.println("recebi token");
					synchronized (data) {
						data.send("token");
			
						data.notifyAll();
					}
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
	private final Data dataServer;
	private final Data dataOperations;

	Logger  logger;
    Scanner scanner;



    public Client(String host, String Mip , int Mport , String M6ip , int M6port,Data dataServer,Data dataOperations, Logger logger) throws Exception
	{
		this.host    = host;
		this.logger  = logger;
		this.Mip = Mip ;
		this.Mport = Mport;
		this.M6ip = M6ip;
		this.M6port = M6port;
		this.dataServer= dataServer;
		this.dataOperations = dataOperations;
		this.logger = logger;
    }

    @Override
    public  void run()
	{
	try
	{
	    logger.info("client: endpoint running ...\n");

		Socket socket ;
		PrintWriter   out;
		BufferedReader in;
		String result;
		String operation;
		boolean flag;

	    while (true)
		{
		try
		{
				synchronized (dataServer)
				{
					if( !(dataServer.flag))
					{
						try
						{
							System.out.println("Esperando wait");
							dataServer.wait();
							System.out.println("Sai do wait");
							message = dataServer.receive();
						}
						catch(Exception e)
						{
							e.printStackTrace();
						}
					}
					else
					{
						message = dataServer.receive();
					}
				}
				if(message.equals("token"))
				{
					System.out.println("Antes do synchronized dataOperations");
					synchronized (dataOperations)
					{
						System.out.println("Teste flag dataOperations");
						if(dataOperations.flag)
						{
							System.out.println("Flag verdade dataOperations");
							operation = dataOperations.receive();
							dataOperations.notifyAll();
							flag = true;
						}
						else
						{
							System.out.println("Flag falsa");
							flag = false;
						}
					}
					if (flag)
					{
						System.out.println("Mandar operação");
						socket  = new Socket(InetAddress.getByName(M6ip), M6port);
						logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

						out = new PrintWriter(socket.getOutputStream(), true);
						in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

						out.println(operation);
						out.flush();
						System.out.println("PReso aqui ?");
						result = in.readLine();
						System.out.println("Não");

						System.out.printf("result is: %f\n", Double.parseDouble(result));
						socket.close();


					}
					System.out.println("Passar token pra frente");
					socket  = new Socket(InetAddress.getByName(Mip), Mport);
					logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");
					out = new PrintWriter(socket.getOutputStream(), true);
					in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
					out.println("token");
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
    }
}