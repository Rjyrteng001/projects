using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawnManager : MonoBehaviour
{
    public Transform spawnPoint;
    public GameObject enemyPrefab;
    public float spawnDistance = 30f;

    private bool hasSpawned = false;

    private void Update()
    {
        // Assuming your player has a tag "Player"
        GameObject player = GameObject.Find("PlayerMesh");

        if (player != null)
        {
            float playerDistance = Vector3.Distance(player.transform.position, spawnPoint.position);

            if (!hasSpawned && playerDistance <= spawnDistance)
            {
                SpawnEnemy(enemyPrefab);
                hasSpawned = true;
            }
        }
    }

    private void SpawnEnemy(GameObject enemyPrefab)
    {
        Instantiate(enemyPrefab, spawnPoint.position, Quaternion.identity);
    }
}
