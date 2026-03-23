using System;
using System.Collections.Generic;
using System.IO.Ports;
using UnityEngine;

public class TelemetryDecoder : MonoBehaviour
{
    
 

    [Header("Port Settings")]
    public string portName = "COM6";
    public int baudRate = 115200;

    [Header("Live Data (Read Only)")]
    public short aileron;
    public short elevator;
    public short throttle;
    public short rudder;
    public short aux1; // ch5, flight mode
    public short aileron2;

    private SerialPort serialPort;
    private List<byte> buffer = new List<byte>();

    // Update is called once per frame
    void Start()
    {
        try
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 10; // Small timeout to prevent Unity from freezing
            serialPort.Open();
            Debug.Log($"[HIL] Successfully opened port {portName}.");
        }
        catch (Exception e)
        {
            Debug.LogError($"[HIL] Port Error: {e.Message}");
        }
    }
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Update()
    {
    
        if (serialPort == null || !serialPort.IsOpen) return;

        // Check if there is new data in the serial buffer
        if (serialPort.BytesToRead > 0)
        {
            byte[] incoming = new byte[serialPort.BytesToRead];
            serialPort.Read(incoming, 0, incoming.Length);

            // Append the incoming bytes 
            buffer.AddRange(incoming);

          
            ProcessBuffer();
        }
    }

    void ProcessBuffer()
    {

        while (buffer.Count >= 24)
        {

            if (buffer[0] == 0xAA && buffer[1] == 0xBB)
            {
     
                int calculatedChecksum = 0;

        
                for (int i = 0; i < 10; i++)
                {
                    calculatedChecksum += BitConverter.ToInt16(buffer.ToArray(), 2 + (i * 2));
                }

    
                calculatedChecksum &= 0xFFFF;

                // Read the received checksum from the last 2 bytes of the packet
                ushort receivedChecksum = BitConverter.ToUInt16(buffer.ToArray(), 22);

        
                if (calculatedChecksum == receivedChecksum)
                {
                    // Checksum matches
                    aileron = BitConverter.ToInt16(buffer.ToArray(), 2);
                    elevator = BitConverter.ToInt16(buffer.ToArray(), 4);
                    throttle = BitConverter.ToInt16(buffer.ToArray(), 6);
                    rudder = BitConverter.ToInt16(buffer.ToArray(), 8);
                    aux1 = BitConverter.ToInt16(buffer.ToArray(), 10);
                    aileron2 = BitConverter.ToInt16(buffer.ToArray(), 12);

                    // move to the next packet
                    buffer.RemoveRange(0, 24);
                }
                else
                {
                    // If checksum fails
                    buffer.RemoveAt(0);
                }
            }
            else
            {
                // If headers do not match
                buffer.RemoveAt(0);
            }
        }
    }

    // Extremely important: Release the port when Unity quits to avoid locking the COM port
    void OnApplicationQuit()
    {
        if (serialPort != null && serialPort.IsOpen)
        {
            serialPort.Close();
            Debug.Log("[HIL] Port closed.");
        }
    }


}
