using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletController : MonoBehaviour
{
    public float speed;
    public float lifeTime;
    public int bulletDamage;
    public GameObject explosionEffect;
    public float radius;
    public float manaCost = 10.0f;
    public float fireballDamage = 10.0f;

    // Update is called once per frame
    void Update()
    {
        transform.Translate(Vector3.forward * speed * Time.deltaTime);
        lifeTime -= Time.deltaTime;
        if(lifeTime <= 0)
        {
            Destroy(gameObject);
        }
    }
    void Explode()
    {
        GameObject explosion = Instantiate(explosionEffect, transform.position, transform.rotation);
        // Get nearby objects
        Collider[] colliders = Physics.OverlapSphere(transform.position, radius);
        foreach (Collider nearbyObject in colliders)
        {
            EnemyAI enemy = nearbyObject.GetComponent<EnemyAI>();
            BossEnemyAI boss = nearbyObject.GetComponent<BossEnemyAI>();
            if (enemy != null)
            {
                Debug.Log("Enemy");
                enemy.TakeDamage(fireballDamage);
            }

            if (boss != null)
            {
                Debug.Log("Boss");
                boss.TakeDamage(fireballDamage);
            }
        }
        Destroy(explosion, 1f);
        Destroy(gameObject);
    }
    private void OnCollisionEnter(Collision collision)
    {
        Explode();
    }
}
