using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyManager : MonoBehaviour
{
    public GameObject objectToActivate; // Reference to the object you want to activate

    private int totalEnemyCount;
    private int remainingEnemies;

    private void Start()
    {
        // Find all enemy prefabs in the scene at the start
        GameObject[] enemies = GameObject.FindGameObjectsWithTag("Enemy");
        Debug.Log(enemies.Length);
        totalEnemyCount = enemies.Length;
        remainingEnemies = totalEnemyCount;
    }

    public void EnemyDestroyed()
    {
        remainingEnemies--;

        if (remainingEnemies <= 0)
        {
            // Activate the object since all enemies are destroyed
            ActivateObject();
        }
    }

    private void ActivateObject()
    {
        objectToActivate.SetActive(true);
    }
}