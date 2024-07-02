using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BillBoard : MonoBehaviour
{
    private Camera cam;
    private void Start()
    {
        cam = GameObject.FindWithTag("MainCamera").GetComponent<Camera>();
    }
    void LateUpdate()
    {
        transform.LookAt(transform.position + cam.transform.forward);
    }
}
