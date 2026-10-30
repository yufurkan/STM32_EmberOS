using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Rigidbody))]
public class AircraftPhysics : MonoBehaviour
{
    public Transform centerOfMass;

    public float maxThrust = 40f;
    [Range(0f, 1f)] public float throttle = 0f;

    // NEW: propeller thrust fade and body drag
    [Header("Propeller thrust fade and body drag")]
    [Tooltip("Airspeed (m/s) at which propeller thrust drops to zero")]
    public float propMaxSpeed = 28f;
    [Tooltip("Body drag area (m^2) in body axes: x = side, y = vertical, z = forward")]
    public Vector3 bodyCdA = new Vector3(0.03f, 0.03f, 0.006f);

    public List<AeroSurface> aeroSurfaces;

    private Rigidbody rb;

    void Awake()
    {
        rb = GetComponent<Rigidbody>();

        // Set custom Center of Mass
        if (centerOfMass != null)
        {
            rb.centerOfMass = transform.InverseTransformPoint(centerOfMass.position);
            rb.inertiaTensor = new Vector3(0.07f, 0.14f, 0.06f); // x=pitch, y=yaw, z=roll (kg*m^2)
            rb.inertiaTensorRotation = Quaternion.identity;
        }
    }

    void FixedUpdate()
    {
        ApplyThrust();
        ApplyAerodynamics();
    }

    void ApplyThrust()
    {
        // CHANGED: thrust fades linearly to zero at propMaxSpeed
        float vFwd = Vector3.Dot(rb.linearVelocity, transform.forward);
        float thrustFade = Mathf.Clamp01(1f - vFwd / propMaxSpeed);
        rb.AddForce(transform.forward * throttle * maxThrust * thrustFade, ForceMode.Force);
    }

    void ApplyAerodynamics()
    {
        // NEW: velocity of the aircraft relative to the air. There is no wind;
        // to add it later, change only this line: airVel = rb.linearVelocity - wind
        Vector3 airVel = rb.linearVelocity;

        Vector3 totalForce = Vector3.zero;
        Vector3 totalTorque = Vector3.zero;

        foreach (var surface in aeroSurfaces)
        {
            if (surface == null) continue;

            // CHANGED: airVel instead of rb.linearVelocity
            Vector3 force = surface.CalculateForces(airVel, rb.angularVelocity, rb.worldCenterOfMass);
            Vector3 relativePos = surface.transform.position - rb.worldCenterOfMass;

            totalForce += force;
            totalTorque += Vector3.Cross(relativePos, force);
        }

        // NEW: body drag (fuselage, gear, struts), applied at the center of mass so it adds no moment
        Vector3 vBody = transform.InverseTransformDirection(airVel);
        Vector3 dragBody = -0.5f * 1.225f * vBody.magnitude * Vector3.Scale(vBody, bodyCdA);
        totalForce += transform.TransformDirection(dragBody);

        rb.AddForce(totalForce, ForceMode.Force);
        rb.AddTorque(totalTorque, ForceMode.Force);
    }

    // Called by TelemetryDecoder
    public void SetThrottle(float percent)
    {
        throttle = Mathf.Clamp01(percent);
    }
}
