using System.Buffers.Text;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;

public class EnemyAI : MonoBehaviour
{
    public NavMeshAgent agent;
    public Transform player;
    public LayerMask whatIsGround, whatIsPlayer;

    //patrolling
    public Vector3 walkPoint;
    bool walkPointSet;
    public float walkPointRange;

    //attacking
    public Transform shootingPoint;
    public float meleeAttackDamage;
    public float timeBetweenAttacks;
    bool alreadyAttacked;
    public GameObject projectile;
    public float projectileDamage;
    private List<GameObject> activeProjectiles = new List<GameObject>();
    private bool isAttacking = false;


    //states
    public float sightRange;
    public float attackRange;
    public bool playerInSightRange;
    public bool playerInAttackRange;
    private bool isDead = false;

    //health
    public float health;
    public float maxHealth;
    public EnemyHealthbar healthBar;

    //PowerUp spawn
    public Transform powerUpSpawnPosition;
    public GameObject[] powerUps;

    //animations
    private Animator animator;

    //audio
    public AudioSource audioSource;
    public AudioClip[] CombatAudioClips;
    public AudioClip[] DeathAudioClips;

    //Damage Popup
    public GameObject DamagePopupPrefab;

    private void Awake()
    {
        player = GameObject.FindGameObjectWithTag("Player").transform;
        animator = GetComponent<Animator>();
    }

    private void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        health = maxHealth;
        healthBar.SetMaxHealth(maxHealth);

        animator.SetBool("IsAttacking", false);
        animator.SetBool("IsDead", false);

        audioSource.mute = true;
    }

    private void Update()
    {
        playerInSightRange = Physics.CheckSphere(transform.position, sightRange, whatIsPlayer);
        playerInAttackRange = Physics.CheckSphere(transform.position, attackRange, whatIsPlayer);

        // Update animation states based on conditions
        animator.SetBool("IsAttacking", playerInAttackRange);

        if (!playerInSightRange && !playerInAttackRange && !isDead)
        {
            Patrolling();
        }

        else if (playerInAttackRange && !isDead)
        {
            StartCoroutine(AttackCoroutine());
        }

        if (playerInSightRange && !playerInAttackRange && !isDead)
        {
            ChasePlayer();
        }
    }

    private void Patrolling()
    {
        //if walk point not set
        if (!walkPointSet)
        {
            SearchWalkPoint();
        }

        if (walkPointSet)
        {
            agent.SetDestination(walkPoint);
        }

        Vector3 distToWalkPoint = transform.position - walkPoint;

        //if walkpoint reached
        if (distToWalkPoint.magnitude < 1f)
        {
            //automatically find new walkpoint
            walkPointSet = false;
        }
    }

    private void SearchWalkPoint()
    {
        //calculate random point in range
        float randomZ = Random.Range(-walkPointRange, walkPointRange);
        float randomX = Random.Range(-walkPointRange, walkPointRange);

        walkPoint = new Vector3(transform.position.x + randomX, transform.position.y, transform.position.z + randomZ);

        //make sure this random walkpoint is in the map and on ground
        if (Physics.Raycast(walkPoint, -transform.up, 2f, whatIsGround))
        {
            walkPointSet = true;
        }
    }

    private void ChasePlayer()
    {
        agent.SetDestination(player.position);
    }

    private IEnumerator AttackCoroutine()
    {
        if (!isAttacking)
        {
            isAttacking = true;

            // Stop the agent from moving during the attack
            agent.ResetPath();

            // Make enemy look at player
            transform.LookAt(player);

            audioSource.mute = false;

            AudioClip randomClip = CombatAudioClips[Random.Range(0, CombatAudioClips.Length)];

            audioSource.clip = randomClip;

            audioSource.Play(); // This plays the assigned audio clip



            if (projectile != null)
            {
                // Launch Projectile
                GameObject newProjectile = Instantiate(projectile, shootingPoint.position, Quaternion.identity);
                activeProjectiles.Add(newProjectile); // Store the active projectile
                EnemyProjectileCollision projectileCollision = newProjectile.GetComponent<EnemyProjectileCollision>();
                if (projectileCollision != null)
                {
                    projectileCollision.projectileDamage = projectileDamage; // Set the projectile damage
                }

                Rigidbody rb = newProjectile.GetComponent<Rigidbody>();
                rb.AddForce(transform.forward * 20f, ForceMode.Impulse);
                Debug.Log("Projectile fly");

                // Set IsAttacking trigger
                animator.SetTrigger("Attack");
            }
            else
            {
                // Set IsAttacking trigger
                animator.SetTrigger("Attack");
                DamagePlayer(player.gameObject);
                Debug.Log("melee damaged");
            }

            yield return new WaitForSeconds(timeBetweenAttacks);

            ResetAttack();

            isAttacking = false;
        }
    }


    public void DamagePlayer(GameObject player)
    {
        // Apply damage to the player's health
        player.GetComponent<PlayerManagement>().TakeDamage(meleeAttackDamage);
    }


    private void ResetAttack()
    {
        alreadyAttacked = false;
    }

    public void TakeDamage(float damage)
    {
        if (!isDead)
        {
            health -= damage;
            healthBar.SetHealth(health);

            GameObject damagePopup = Instantiate(DamagePopupPrefab, transform.position, Quaternion.identity);
            DamagePopupController popupController = damagePopup.GetComponent<DamagePopupController>();

            popupController.ShowDamagePopup(damage);

            alreadyAttacked = false;

            if (health <= 0)
            {
                isDead = true;
                animator.SetBool("IsDead", true);
                animator.SetTrigger("Die");

                if (DeathAudioClips.Length > 0)
                {
                    AudioClip randomClip = DeathAudioClips[Random.Range(0, DeathAudioClips.Length)];
                    audioSource.clip = randomClip;

                    audioSource.Play(); // Play the death audio
                    if (agent != null)
                    {
                        agent.isStopped = true;
                    }
                }
                Invoke(nameof(DestroyEnemy), 1f);
            }
        }
    }

    private void DestroyEnemy()
    {
        // Check if a power-up should be dropped (20% chance)
        if (Random.Range(0f, 1f) <= 0.35f)
        {
            // Randomly choose a power-up from the array
            int randomPowerUpIndex = Random.Range(0, powerUps.Length);
            GameObject selectedPowerUp = powerUps[randomPowerUpIndex];

            // Spawn the chosen power-up at the enemy's position
            GameObject spawnedPowerUp = Instantiate(selectedPowerUp, powerUpSpawnPosition.position, Quaternion.identity);
        }

        FindObjectOfType<EnemyManager>().EnemyDestroyed();
        // Destroy the enemy
        Destroy(gameObject, 1f);
    }



    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(transform.position, attackRange);
        Gizmos.color = Color.yellow;
        Gizmos.DrawWireSphere(transform.position, sightRange);
    }
}