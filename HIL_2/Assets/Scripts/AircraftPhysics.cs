using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class AircraftPhysics : MonoBehaviour
{
    public Transform centerOfMass;

    public float maxThrust = 40f;
    [Range(0f, 1f)] public float throttle = 0f;

    public List<AeroSurface> aeroSurfaces;

    private Rigidbody rb;

    void Awake()
    {
        rb = GetComponent<Rigidbody>();

        // Set custom Center of Mass
        if (centerOfMass != null)
        {
            rb.centerOfMass = transform.InverseTransformPoint(centerOfMass.position);
        }
    }

    void FixedUpdate()
    {
        ApplyThrust();
        ApplyAerodynamics();
    }

    void ApplyThrust()
    {
        rb.AddForce(transform.forward * throttle * maxThrust, ForceMode.Force);
    }

    void ApplyAerodynamics()
    {
        Vector3 totalForce = Vector3.zero;
        Vector3 totalTorque = Vector3.zero;

        foreach (var surface in aeroSurfaces)
        {
            if (surface == null) continue;

            Vector3 force = surface.CalculateForces(rb.linearVelocity, rb.angularVelocity, rb.worldCenterOfMass);
            Vector3 relativePos = surface.transform.position - rb.worldCenterOfMass;

            totalForce += force;
            totalTorque += Vector3.Cross(relativePos, force);
        }

        rb.AddForce(totalForce, ForceMode.Force);
        rb.AddTorque(totalTorque, ForceMode.Force);
    }

    // Called by TelemetryDecoder
    public void SetThrottle(float percent)
    {
        throttle = Mathf.Clamp01(percent);
    }
}