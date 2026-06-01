using UnityEngine;

[CreateAssetMenu(fileName = "NewAeroConfig", menuName = "Physics/AeroConfig")]
public class AeroConfig : ScriptableObject
{
    [Header("Profile Parameters")]
    public float liftSlope = 6.28f;
    public float skinFriction = 0.02f;


    public float zeroLiftAoA = 0f;

    [Header("Stall Dynamics")]
    public float stallAngleHigh = 15f;
    public float stallAngleLow = -15f;

    public float postStallLiftRatio = 0.5f;
}