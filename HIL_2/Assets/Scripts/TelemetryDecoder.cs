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
    public short aux1;
    public short aileron2;

    [Header("HIL Jitter Filter (Low-Pass)")]
    [Tooltip("0 = High smoothing, 1 = No filter")]
    [Range(0.01f, 1f)]
    public float emaAlpha = 0.2f;
    private float smAileron = 1500, smElevator = 1500, smRudder = 1500, smThrottle = 1000;

    [Header("Physics Connections")]
    public AircraftPhysics aircraftPhysics;
    public AeroSurface rightAileronAero;
    public AeroSurface leftAileronAero;
    public AeroSurface elevatorAero;
    public AeroSurface rudderAero;

    [Header("Airplane 3D Models")]
    public Transform rightAileronTransform;
    public Transform leftAileronTransform;
    public Transform elevatorTransform;
    public Transform rudderTransform;

    private Quaternion startRightAileron, startLeftAileron, startElevator, startRudder;

    [Header("Max Angles")]
    public float maxAngle = 30f;

    [Header("Propeller")]
    public Transform propellerTransform;
    public float maxRPM = 12100f;

    private SerialPort serialPort;
    private List<byte> buffer = new List<byte>();

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

        // Apply EMA filter to mitigate hardware jitter
        smAileron = Mathf.Lerp(smAileron, aileron, emaAlpha);
        smElevator = Mathf.Lerp(smElevator, elevator, emaAlpha);
        smRudder = Mathf.Lerp(smRudder, rudder, emaAlpha);
        smThrottle = Mathf.Lerp(smThrottle, throttle, emaAlpha);

        // Map PWM to degrees
        float aileronAngle = Map(smAileron, 1000, 2000, -maxAngle, maxAngle);
        float elevatorAngle = Map(smElevator, 1000, 2000, -maxAngle, maxAngle);
        float rudderAngle = Map(smRudder, 1000, 2000, -maxAngle, maxAngle);

        // Pass calculated angles (in radians) to physics scripts
        if (rightAileronAero != null) rightAileronAero.SetFlapAngle(aileronAngle * Mathf.Deg2Rad);//
        if (leftAileronAero != null) leftAileronAero.SetFlapAngle(aileronAngle * Mathf.Deg2Rad); 
        if (elevatorAero != null) elevatorAero.SetFlapAngle(elevatorAngle * Mathf.Deg2Rad);
        if (rudderAero != null) rudderAero.SetFlapAngle(rudderAngle * Mathf.Deg2Rad);

        // Visual model rotations
        if (rightAileronTransform != null)
            rightAileronTransform.localRotation = startRightAileron * Quaternion.Euler(aileronAngle, 0, 0);

        if (leftAileronTransform != null)
            leftAileronTransform.localRotation = startLeftAileron * Quaternion.Euler(aileronAngle, 0, 0);

        if (elevatorTransform != null)
            elevatorTransform.localRotation = startElevator * Quaternion.Euler(elevatorAngle, 0, 0);

        if (rudderTransform != null)
            rudderTransform.localRotation = startRudder * Quaternion.Euler(0, rudderAngle, 0);

        // Calculate thrust and propeller RPM
        float throttlePercent = Mathf.Clamp01(Map(smThrottle, 1000, 2000, 0f, 1f));

        if (aircraftPhysics != null) aircraftPhysics.SetThrottle(throttlePercent);

        if (propellerTransform != null)
        {
            float currentRPM = throttlePercent * maxRPM;
            float rotationSpeed = currentRPM * 6f;
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
                ushort receivedChecksum = BitConverter.ToUInt16(buffer.ToArray(), 22);

                if (calculatedChecksum == receivedChecksum)
                {
                    aileron = BitConverter.ToInt16(buffer.ToArray(), 2);
                    elevator = BitConverter.ToInt16(buffer.ToArray(), 4);
                    throttle = BitConverter.ToInt16(buffer.ToArray(), 6);
                    rudder = BitConverter.ToInt16(buffer.ToArray(), 8);
                    aux1 = BitConverter.ToInt16(buffer.ToArray(), 10);
                    aileron2 = BitConverter.ToInt16(buffer.ToArray(), 12);

                    buffer.RemoveRange(0, 24);
                }
                else
                {
                    buffer.RemoveAt(0);
                }
            }
            else
            {
                buffer.RemoveAt(0);
            }
        }
    }

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