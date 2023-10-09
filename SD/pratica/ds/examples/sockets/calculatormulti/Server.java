package ds.examples.sockets.calculatormulti;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Scanner;

public class Server {
    private ServerSocket server;

    public Server(String ipAddress) throws Exception {
        this.server = new ServerSocket(0, 1, InetAddress.getByName(ipAddress));
    }

    private void listen() throws Exception {
	while(true) {
	    Socket client = this.server.accept();
	    String clientAddress = client.getInetAddress().getHostAddress();
	    System.out.printf("\r\nnew connection from %s\n", clientAddress);
	    new Thread(new ConnectionHandler(clientAddress, client)).start();
	}
    }
    
    public InetAddress getSocketAddress() {
	return this.server.getInetAddress();
    }
        
    public int getPort() {
	return this.server.getLocalPort();
    }
    
    public static void main(String[] args) throws Exception {
	Server app = new Server(args[0]);
	System.out.printf("\r\nrunning server: host=%s @ port=%d\n",
	    app.getSocketAddress().getHostAddress(), app.getPort());
	app.listen();
    }
}



class ConnectionHandler implements Runnable {
    String clientAddress;
    Socket clientSocket;    

    public ConnectionHandler(String clientAddress, Socket clientSocket) {
	this.clientAddress = clientAddress;
	this.clientSocket  = clientSocket;    
    }

    @Override
    public void run() {
	/*
	 * prepare socket I/O channels
	 */
	try {
	    BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));    
	    PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
	
	    while(true) {
		/* 
		 * receive command 
		 */
		String command;
		if( (command = in.readLine()) == null)
		    break;
		else
		    System.out.printf("message from %s : %s\n", clientAddress, command);		      	                    /*
																     * process command 
																     */
		Scanner sc = new Scanner(command).useDelimiter(":");
		String  op = sc.next();
		switch(op) {
		case "add":
			double  x  = Double.parseDouble(sc.next());
			double  y  = Double.parseDouble(sc.next());
			double result ;
			result = x + y; 
			out.println(String.valueOf(result));
			break;
		case "sub": 
			double  x1  = Double.parseDouble(sc.next());
			double  y1  = Double.parseDouble(sc.next());
			double result1 ;
			result1 = x1 - y1; 
			out.println(String.valueOf(result1));
			break;
		case "mul": 
			double  x2  = Double.parseDouble(sc.next());
			double  y2  = Double.parseDouble(sc.next());
			double result2 ;
			result2 = x2 * y2; 
			out.println(String.valueOf(result2));
			break;
		case "div": 
			double  x3  = Double.parseDouble(sc.next());
			double  y3  = Double.parseDouble(sc.next());
			double result3 ;
			result3 = x3 / y3; 
			out.println(String.valueOf(result3));
			break;
		case "len":
			String x4 = sc.next();
			int result4 = x4.length() ;
			out.println(String.valueOf(result4));
			break;
		case "comp":
			String x5 = sc.next();
			String y5 = sc.next();
			if(x5.equals(y5)){
				out.println("true");
			}
			else
			{
				out.println("false");
			}
			break;
		case "cat":
			String x6 = sc.next();
			String y6 = sc.next();
			String result6 = x6 + y6;
			out.println(result6);
			break;
		case "break":
			String x7 = sc.next();
			String y7 = sc.next();
			String[] arr = x7.split(y7);
			String result7 ="";
			for (int i = 0; i <arr.length; i++)
			{
				System.out.printf(arr[i] + "\n");
				result7 = result7 + arr[i] + " ";

			}
			System.out.printf(result7);
			out.println(result7);

			break;
		}
		out.flush();
	    }
	}catch(Exception e) {
	    e.printStackTrace();
	}
    }
}
