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


//Classe para passar Informação 
class Data 
{
	//Guarda Operações
    private ArrayList<String> arr = new ArrayList<String>() ;
	public boolean flag;

	//Adicionar operação e sinalizar que tem operações
    public void send(String message)
	{
		arr.add(message);
		this.flag = true;
    }
	//Mandar array e marcar flag como falsa
    public ArrayList<String> receive() 
	{
		this.flag =false;
        return this.arr;
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
		//Creating Server
		new Thread(new ServerPeer("localhost", Integer.parseInt(args[0]),peer.logger,dataOperations,args[1],Integer.parseInt(args[2]),args[3],Integer.parseInt(args[4]))).start();
		//Creating Operations
		new Thread(new CreateOperations(4,dataOperations)).start();

		//Ve sê é pra começar , neste caso manda uma mensagem com o token
		if (args.length == 6)
		{
			try
			{
				PrintWriter   out;
				BufferedReader in;
				Socket client ;
				Socket socket;
				socket  = new Socket(InetAddress.getByName("localhost"),Integer.parseInt(args[0]));
				peer.logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

				out = new PrintWriter(socket.getOutputStream(), true);
				in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

				out.println("token");
			}
			catch(Exception e)
			{
				e.printStackTrace();

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
					Thread.sleep(t);
					//Gerar os inteiros
					op = rand.nextInt(4);
					x = rand.nextInt();
					y = rand.nextInt();
					synchronized (data) 
					{
						//Guardar operação no array
						data.send(operations[op] + ":" + x + ":" + y);
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
		this.dataOperations= dataOperations;
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
			//logger.info("server: endpoint running at port " + port + " ...");
			PrintWriter   out;
			BufferedReader in;
			Socket client ;
			Socket socket;
			String command;
			ArrayList<String> operations;
			String result ="";
			while(true) 
			{
				try {

					client = server.accept();
					//logger.info("server: new connection from " + clientAddress);
					try
					{
						in = new BufferedReader(new InputStreamReader(client.getInputStream()));
						command = in.readLine();
						client.close();
						//logger.info("server: message from host " + clientAddress + "[command = " + command + "]");
							try
							{
								// Obter lock , para não adicionar novas operacoes
								synchronized (dataOperations)
								{
									//Checar se tem operações pra serem feitas
									if(dataOperations.flag)
									{
										operations = dataOperations.receive();
										socket  = new Socket(InetAddress.getByName(M6ip), M6port);
										//logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

										out = new PrintWriter(socket.getOutputStream(), true);
										in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

										for(String s : operations)
										{
											out.println(s);
											out.flush();
											result = in.readLine();
											// Imprimir resultado
											System.out.printf("result is: %f\n", Double.parseDouble(result));
										}
										//Retirar as operações antigas
										operations.clear();
										socket.close();
									}
									//Passar o token pra frente
									socket  = new Socket(InetAddress.getByName(Mip), Mport);
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