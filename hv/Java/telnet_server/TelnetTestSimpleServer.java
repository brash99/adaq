/*
 * Copyright 2003-2004 The Apache Software Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//package org.apache.commons.net.telnet;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.util.*;
import java.io.*;

/***
 * Simple TCP server.
 * Waits for connections on a TCP port in a separate thread.
 * <p>
 * @author Bruno D'Avanzo
 ***/
public class TelnetTestSimpleServer implements Runnable
{
    ServerSocket serverSocket = null;
    Socket clientSocket = null;
    Thread listener = null;

    /***
     * test of client-driven subnegotiation.
     * <p>
     * @param port - server port on which to listen.
     ***/
    public TelnetTestSimpleServer(int port) throws IOException
    {
        serverSocket = new ServerSocket(port);

        listener = new Thread (this);

        listener.start();
    }

    /***
     * Run for the thread. Waits for new connections
     ***/
    public void run()
    {
        boolean bError = false;
	boolean allow = true;
        while(!bError)
        {
            try
            {
                clientSocket = serverSocket.accept();
		System.out.println("Client Connected ...");
                synchronized (clientSocket)
                {
                    try
                    {
			// clientSocket.wait(); //
			/****/
			//DataInputStream din = new DataInputStream(clientSocket.getInputStream());
			BufferedReader din = new BufferedReader(
                                                   new InputStreamReader(clientSocket.getInputStream()));
			DataOutputStream dout = new DataOutputStream(clientSocket.getOutputStream());

			while(allow)
			    {
				String strCommand;
				//strCommand=din.readUTF();
				strCommand=din.readLine();
				System.out.println(strCommand);
				if(strCommand.equals("quit"))
				    {
					allow=false;
				    }
				else
				    {
					Runtime rt=Runtime.getRuntime();
					
					Process p=rt.exec("/home/romanip/Java/telnet_server/TelnetServer.parser " + strCommand);
					
					String stdout=new String("");
					String st;
					BufferedReader dstdin  = new BufferedReader(new InputStreamReader(p.getInputStream()));
					while((st=dstdin.readLine())!=null)
					    {
						stdout=stdout +st + "\n";
					    }
					dstdin.close();
					//	dout.writeUTF(stdout);                        
					dout.write(stdout.getBytes() ,0, stdout.length() );                        
				    }                        
			    }
			/*****/
                    }
                    catch (Exception e)
                    {
                        System.err.println("Exception in wait, "+ e.getMessage());
                    }
                    try
                    {
                        clientSocket.close();
                    }
                    catch (Exception e)
                    {
                        System.err.println("Exception in close, "+ e.getMessage());
                    }
                }
            }
            catch (IOException e)
            {
                bError = true;
            }
        }

        try
        {
            serverSocket.close();
        }
        catch (Exception e)
        {
            System.err.println("Exception in close, "+ e.getMessage());
        }
    }


    /***
     * Disconnects the client socket
     ***/
    public void disconnect()
    {
        synchronized (clientSocket)
        {
            try
            {
                clientSocket.notify();
            }
            catch (Exception e)
            {
                System.err.println("Exception in notify, "+ e.getMessage());
            }
        }
    }

    /***
     * Stop the listener thread
     ***/
    public void stop()
    {
        listener.interrupt();
        try
        {
            serverSocket.close();
        }
        catch (Exception e)
        {
            System.err.println("Exception in close, "+ e.getMessage());
        }
    }

    /***
     * Gets the input stream for the client socket
     ***/
    public InputStream getInputStream() throws IOException
    {
        if(clientSocket != null)
        {
            return(clientSocket.getInputStream());
        }
        else
        {
            return(null);
        }
    }

    /***
     * Gets the output stream for the client socket
     ***/
    public OutputStream getOutputStream() throws IOException
    {
        if(clientSocket != null)
        {
            return(clientSocket.getOutputStream());
        }
        else
        {
            return(null);
        }
    }

   /****** main ********/
    public static void main(String[] args) {
	int def_port=2003; //default  port 2003 (on my machine, open port 23 returns exception "Permission denied"
	try
	    {	       
	       TelnetTestSimpleServer tserver = new TelnetTestSimpleServer(def_port);	       
	    }
	catch (IOException e)
	    {
		System.err.println("Exception in main, "+ e.getMessage());
	    }
    }
}