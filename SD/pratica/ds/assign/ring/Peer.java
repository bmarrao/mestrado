package ds.assign.ring;

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
import java.util.Random;

import ds.examples.sockets.poisson.PoissonProcess;

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
		new Thread(new Server(args[0], Integer.parseInt(args[1]), Integer.parseInt(arg[2]), peer.logger)).start();
		new Thread(new Client(args[0], peer.logger)).start();
    }
}

class Server implements Runnable {
    String       host;
    int          port;
    ServerSocket server;
    Logger       logger;
    
    public Server(String host, int port, Logger logger) throws Exception {
	this.host   = host;
	this.port   = port;
	this.logger = logger;
    server = new ServerSocket(port, 1, InetAddress.getByName(host));
    }

    @Override
    public void run() {
	try {
	    logger.info("server: endpoint running at port " + port + " ...");
	    while(true) {
		try {
		    Socket client = server.accept();
		    String clientAddress = client.getInetAddress().getHostAddress();
		    logger.info("server: new connection from " + clientAddress);
		    new Thread(new Connection(clientAddress, client, logger)).start();
		}catch(Exception e) {
		    e.printStackTrace();
		}    
	    }
	} catch (Exception e) {
	     e.printStackTrace();
	}
    }
}

class Connection implements Runnable {
    String clientAddress;
    Socket clientSocket;
    Logger logger;

    public Connection(String clientAddress, Socket clientSocket, Logger logger) {
	this.clientAddress = clientAddress;
	this.clientSocket  = clientSocket;
	this.logger        = logger;
    }

    @Override
    public void run() {
	/*
	 * prepare socket I/O channels
	 */
	try {
	    BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));    
	    PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
	
	    String command;
	    command = in.readLine();
	    logger.info("server: message from host " + clientAddress + "[command = " + command + "]");
	    /*
	     * parse command
	     */
	    Scanner sc = new Scanner(command);
	    String  op = sc.next();	    
	    double  x  = Double.parseDouble(sc.next());
	    double  y  = Double.parseDouble(sc.next());
	    double  result = 0.0; 
	    /*
	     * execute op
	     */
	    switch(op) {
	    case "add": result = x + y; break;
	    case "sub": result = x - y; break;
	    case "mul": result = x * y; break;
	    case "div": result = x / y; break;
	    }  
	    /*
	     * send result
	     */
	    out.println(String.valueOf(result));
	    out.flush();
	    /*
	     * close connection
	     */
	    clientSocket.close();
	} catch(Exception e) {
	    e.printStackTrace();
	}
    }
}

class Client implements Runnable {
    String  host;
    Logger  logger;
    Scanner scanner;
	PoissonProcess pp;
    
    public Client(String host, Logger logger) throws Exception {
		this.host    = host;
		this.logger  = logger; 
		this.scanner = new Scanner(System.in);
		this.pp = new PoissonProcess(5, new Random(0));

    }

    @Override 
    public void run() {
	try {
	    logger.info("client: endpoint running ...\n");	
	    /*
	     * send messages such as:
	     *   - ip port add x y
	     *   - ip port sub x y
	     *   - ip port mul x y
	     *   - ip port div x y
	     * where x, y are floting point values and ip is the address of the server
	     */

		int t;
		Random rand = new Random();
		int op ;
		int x;
		int y;
		String[]operations = {"add","sub","mul","div"};
	    while (true) {
		try {

			try{
				t = (int)(this.pp.timeForNextEvent()*60.0*1000.0);
				Thread.sleep(t);
				Socket socket  = new Socket(InetAddress.getByName("localhost"), 3000);
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
			}
			catch(Exception e) {
		    e.printStackTrace();
			}   
			/*

		    System.out.print("$ ");
		    String server  = scanner.next();
		    String port    = scanner.next();
		    String command = scanner.nextLine();

		    Socket socket  = new Socket(InetAddress.getByName(server), Integer.parseInt(port));
		    logger.info("client: connected to server " + socket.getInetAddress() + "[port = " + socket.getPort() + "]");

		    PrintWriter   out = new PrintWriter(socket.getOutputStream(), true);
		    BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));    

		    out.println(command);
		    out.flush();	    

		    String result = in.readLine();
						//System.out.printf("" + this.pp.events());

		    socket.close();
			*/

		} catch(Exception e) {
		    e.printStackTrace();
		}   
	    }
	} catch(Exception e) {
	    e.printStackTrace();
	}   	    
    }
}
