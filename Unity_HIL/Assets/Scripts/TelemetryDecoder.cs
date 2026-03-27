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

    [Header("Airplane 3D Models")]
    public Transform rightAileronTransform;
    public Transform leftAileronTransform;
    public Transform elevatorTransform;
    public Transform rudderTransform;

    private Quaternion startRightAileron;
    private Quaternion startLeftAileron;
    private Quaternion startElevator;
    private Quaternion startRudder;

    [Header("Max Angles")]
    public float maxAngle = 30f;

    [Header("Propeller")]
    public Transform propellerTransform;
    public float maxRPM = 12100f; // 550KV * 22V

    private SerialPort serialPort;
    private List<byte> buffer = new List<byte>();

    // Update is called once per frame
    void Start()
    {
        if (rightAileronTransform != null) startRightAileron = rightAileronTransform.localRotation;
        if (leftAileronTransform != null) startLeftAileron = leftAileronTransform.localRotation;
        if (elevatorTransform != null) startElevator = elevatorTransform.localRotation;
        if (rudderTransform != null) startRudder = rudderTransform.localRotation;

        try
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.ReadTimeout = 10;
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

        if (serialPort.BytesToRead > 0)
        {
            byte[] incoming = new byte[serialPort.BytesToRead];
            serialPort.Read(incoming, 0, incoming.Length);
            buffer.AddRange(incoming); 
            ProcessBuffer();
        }

        float aileronAngle = Map(aileron, 1000, 2000, -maxAngle, maxAngle);
        float elevatorAngle = Map(elevator, 1000, 2000, -maxAngle, maxAngle);
        float rudderAngle = Map(rudder, 1000, 2000, -maxAngle, maxAngle);

        // Önce sadece Z eksenini test et
        if (rightAileronTransform != null)
            rightAileronTransform.localRotation = startRightAileron * Quaternion.Euler(aileronAngle, 0, 0);

        if (leftAileronTransform != null)
            leftAileronTransform.localRotation = startLeftAileron * Quaternion.Euler(aileronAngle, 0, 0);

        if (elevatorTransform != null)
            elevatorTransform.localRotation = startElevator * Quaternion.Euler(elevatorAngle, 0, 0);

        if (rudderTransform != null)
            rudderTransform.localRotation = startRudder * Quaternion.Euler(0, rudderAngle, 0);


        
        float throttlePercent = Mathf.Clamp01(Map(throttle, 1000, 2000, 0f, 1f));

      
        float currentRPM = throttlePercent * maxRPM;

      
        float rotationSpeed = currentRPM * 6f;

        if (propellerTransform != null)
        {
            propellerTransform.Rotate(0, 0, rotationSpeed * Time.deltaTime);
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

    float Map(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


}
