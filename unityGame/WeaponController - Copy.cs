using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WeaponController : MonoBehaviour
{
    public bool CanAttack = true;
    public float AttackCooldown = 1f;
    public bool isAttacking = false;
    public GameObject particleSysPrefab;
    public AudioSource swordSound;
    public AudioClip enemyHit;
    public Transform attackPoint;
    public float attackRange = 0.5f;
    public LayerMask enemyLayers;
    public float swordDamage = 10.0f;
    public GunController gun;

    void Start()
    {
        swordDamage = PlayerPrefs.GetFloat("SwordDamage", swordDamage);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if (CanAttack)
            {
                swordSound.Play();
                StartCoroutine(attackCoroutine());
                StartCoroutine(ResetAttackBool());
            }
        }
    }

    public IEnumerator attackCoroutine()
    {
        isAttacking = true;
        Debug.Log("Attack");
        CanAttack = false;

        //Detect enemies in sphere of attack
        Collider[] hitenemies = Physics.OverlapSphere(attackPoint.position, attackRange, enemyLayers);

        foreach (Collider nearbyObject in hitenemies)
        {
            EnemyAI enemy = nearbyObject.GetComponent<EnemyAI>();
            BossEnemyAI boss = nearbyObject.GetComponent<BossEnemyAI>();
            if (enemy != null)
            {
                enemy.TakeDamage(swordDamage);
                Debug.Log("Enemy");
                swordSound.PlayOneShot(enemyHit);
                GameObject hit = Instantiate(particleSysPrefab, new Vector3(enemy.transform.position.x,
                            enemy.transform.position.y, enemy.transform.position.z), enemy.transform.rotation);
                Destroy(hit, 1.0f);

            }
            if (boss!= null)
            {
                boss.TakeDamage(swordDamage);
                Debug.Log("Boss");
                swordSound.PlayOneShot(enemyHit);
                GameObject hit = Instantiate(particleSysPrefab, new Vector3(boss.transform.position.x,
                            boss.transform.position.y, boss.transform.position.z), boss.transform.rotation);
                Destroy(hit, 1.0f);
            }

        }
        yield return new WaitForSeconds(AttackCooldown);
    }

    IEnumerator ResetAttackCooldown()
    {
        StartCoroutine(ResetAttackBool());
        yield return new WaitForSeconds(AttackCooldown);
        CanAttack = true;
    }

    IEnumerator ResetAttackBool()
    {
        yield return new WaitForSeconds(1f);
        CanAttack = true;
        isAttacking = false;
    }

    private void OnDrawGizmosSelected()
    {
        if (attackPoint == null)
            return;
        Gizmos.DrawWireSphere(attackPoint.position, attackRange);
    }
}
