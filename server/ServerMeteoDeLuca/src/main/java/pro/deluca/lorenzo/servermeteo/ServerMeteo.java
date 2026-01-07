package pro.deluca.lorenzo.servermeteo;

import java.io.*;
import java.net.*;
import java.time.LocalDateTime;
import java.time.temporal.ChronoField;

/**
 * @author Lorenzo De Luca
 */
public class ServerMeteo {
    static String hostname="deluca.pro";
    static String toSend;
    static int portLocal=9998;
    static int portRemote=9999;
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws UnknownHostException, SocketException, InterruptedException, IOException {
        InetAddress address = InetAddress.getByName("192.168.8.7"); 
        InetAddress addressStation = InetAddress.getByName("192.168.8.214"); 
        InetAddress broadcast = InetAddress.getByName("192.168.8.255"); 
        String test="init";
        DatagramSocket socket = new DatagramSocket(portLocal);
        System.out.println("Server started successfully");
        DatagramPacket requestInit = new DatagramPacket(test.getBytes(),test.length(), broadcast, portRemote);
        socket.send(requestInit);
        while (true) {
            //data update recalc
            LocalDateTime now = LocalDateTime.now();
            int year = now.getYear();
            int month = now.getMonthValue();
            int day = now.getDayOfMonth();
            int hour = now.getHour();
            int minute = now.getMinute();
            int second = now.getSecond();
            int millis = now.get(ChronoField.MILLI_OF_SECOND);
            //creating the send request(time)
            toSend=Integer.toString(day)+":";
            toSend+=Integer.toString(month)+":";
            toSend+=Integer.toString(year)+":";
            toSend+=Integer.toString(hour)+":";
            toSend+=Integer.toString(minute)+":";
            toSend+=Integer.toString(second)+"\n";
            //data sending
            DatagramPacket request = new DatagramPacket(toSend.getBytes(),toSend.length(), address, portRemote);
            socket.send(request);
            //data reception
            byte[] buffer = new byte[512];
            DatagramPacket response = new DatagramPacket(buffer, buffer.length);
            socket.receive(response);
            String quote = new String(buffer, 0, response.getLength());
            //data elaborantion and storing in the csv file(append mode) data are alreadyCSVformatted so they are ready to be stored
            try (FileWriter writer = new FileWriter("weather.conditions", true);
		 BufferedWriter bw = new BufferedWriter(writer)) {
		bw.write(quote);
                } catch (IOException e) {
                        System.err.format("IOException: %s%n", e);
                }
            
            //Console log
            System.out.println(quote);
            System.out.println();

            Thread.sleep(1000);
        }
    }
}
