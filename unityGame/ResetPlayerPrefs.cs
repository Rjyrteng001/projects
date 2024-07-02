using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResetPlayerPrefs : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        // Set upgraded variables to default values
        PlayerPrefs.SetFloat("SwordDamage", 10.0f);
        PlayerPrefs.SetFloat("PlayerMaxHealth", 100.0f);
        PlayerPrefs.SetFloat("PlayerHealth", 100.0f);
        PlayerPrefs.SetFloat("PlayerMaxMana", 100.0f);
        PlayerPrefs.SetFloat("PlayerMoveSpeed", 6.0f);
        PlayerPrefs.Save();
    }

}
