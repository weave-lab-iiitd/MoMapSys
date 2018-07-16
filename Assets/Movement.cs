using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.IO.Ports;
using System.IO;
using System.Linq;

public class Movement : MonoBehaviour {

    SerialPort sp = new SerialPort("COM4", 9600);
    float leftCalfAngle, leftThighAngle;
    public Transform leftCalf;
    public Transform leftThigh;
    float l_c_a_old = 0;
    float l_t_a_old = 0;
    // Use this for initialization
	void Start () {
        sp.ReadTimeout = 15;
        sp.Open();
        sp.DtrEnable = true;
        sp.RtsEnable = true;
	}

    // Update is called once per frame
    void Update()
    {

        if (sp.IsOpen)
        {
            try
            {
                serialEvent(sp);

                leftCalf.Rotate(0, 0, leftCalfAngle-l_c_a_old);
                leftThigh.Rotate(0, 0, leftThighAngle-l_t_a_old);
                l_c_a_old = leftCalfAngle;
                l_t_a_old = leftThighAngle;
                //leftCalf.position = leftCalf.position + new Vector3(0f, -leftCalf.position.y + leftFootCalf, 0.0f);
                //leftThigh.position = leftThigh.position + new Vector3(0f, -leftThigh.position.y + leftFootThigh, 0.0f);
            }
            catch (System.Exception)
            {
                throw;
            }

        }

    }
    void serialEvent(SerialPort myPort)
    {
            
        string MyString = myPort.ReadLine();
        if(MyString != null)
        {
            MyString = MyString.Trim();
            string[] inputs = MyString.Split(',');
            if(inputs.Length == 2)
            {
                leftCalfAngle = float.Parse(inputs[1]);
                leftThighAngle = float.Parse(inputs[0]);
                Debug.Log("thigh:");
                Debug.Log(leftThighAngle);
                Debug.Log("calf:");
                Debug.Log(leftCalfAngle);
            }
        }
    }
}
