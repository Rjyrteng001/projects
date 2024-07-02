using UnityEngine;
using System.Collections;
using UnityEngine.AI;
using UnityEngine.SceneManagement;

public class BossEnemyAI : MonoBehaviour
{
    public NavMeshAgent agent;
    public Transform player;
    public LayerMask whatIsGround, whatIsPlayer;

    //patrolling
    public Vector3 walkPoint;
    bool walkPointSet;
    public float walkPointRange;

    //attacking
    private bool isAttacking = false;
    public Transform shootingPoint;
    public float meleeAttackDamage;
    public float timeBetweenAttacks;
    bool alreadyAttacked;

    // Meteor shower
    public GameObject meteorPrefab;  
    public float meteorRainCooldown = 5f;
    private bool isMeteorRaining = false;
    public int numberOfMeteors = 4;
    public float timeBetweenMeteors = 1f;
    public float meteorDamage;

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

    //animations
    private Animator animator;

    //Damage Popup
    public GameObject DamagePopupPrefab;

    //impact effect
    public GameObject impactEffectPrefab;
    public float impactEffectDuration = 1f;

    //audio
    public AudioSource audioSource;
    public AudioClip[] CombatAudioClips;
    public AudioClip[] DeathAudioClips;
    public AudioClip[] MeteorSpawnClips;

    private void Awake()
    {
        player = GameObject.FindGameObjectWithTag("Player").transform;
        animator = GetComponent<Animator>();
    }

    private void Start()
    {
        health = maxHealth;
        healthBar.SetMaxHealth(maxHealth);

        animator.SetBool("IsAttacking", false);
        animator.SetBool("IsDead", false);
    }

    private void Update()
    {
        playerInSightRange = Physics.CheckSphere(transform.position, sightRange, whatIsPlayer);
        playerInAttackRange = Physics.CheckSphere(transform.position, attackRange, whatIsPlayer);

        // Update animation states based on conditions
        animator.SetBool("IsAttacking", playerInAttackRange);

        if (!playerInSightRange && !playerInAttackRange && !isDead)
        {
            ChasePlayer();
        }

        else if (playerInAttackRange && !isDead)
        {
            if (!isAttacking) // Check if not already attacking
            {
                StartCoroutine(AttackCoroutine());
            }
        }

        else if (CanStartMeteorRain())
        {
            Debug.Log("Meteor");
            StartMeteorRain();
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
        isAttacking = true;

        // Stop the agent from moving during the attack
        agent.ResetPath();

        // Make enemy look at player
        transform.LookAt(player);

        if (!alreadyAttacked)
        {
            // Check if the player is within the AOE damage area
            if (Physics.CheckSphere(shootingPoint.position, attackRange, whatIsPlayer))
            {
                DamagePlayer(player.gameObject);
                Debug.Log("AOE damaged");

                AudioClip randomClip = CombatAudioClips[Random.Range(0, CombatAudioClips.Length)];
                audioSource.clip = randomClip;
                audioSource.Play(); // This plays the assigned audio clip

                // Instantiate and play impact effect
                if (impactEffectPrefab != null)
                {
                    Quaternion impactRotation = Quaternion.Euler(-90, 0, 0);
                    GameObject impactEffect = Instantiate(impactEffectPrefab, shootingPoint.position, impactRotation);
                    Destroy(impactEffect, impactEffectDuration); // Destroy effect after it finishes playing
                }
            }

            //set IsAttacking trigger
            animator.SetTrigger("Attack");

            alreadyAttacked = true;

            yield return new WaitForSeconds(timeBetweenAttacks); // Delay between attacks

            ResetAttack();

            isAttacking = false; // Set attacking state back to false
        }
    }


    public void DamagePlayer(GameObject player)
    {
        // Apply damage to the player's health
        player.GetComponent<PlayerManagement>().TakeDamage(meleeAttackDamage);
    }

    private bool CanStartMeteorRain()
    {
        return !isMeteorRaining && playerInSightRange && !isDead;
    }

    private void StartMeteorRain()
    {
        isMeteorRaining = true;
        StartCoroutine(MeteorRainCoroutine());
    }

    private IEnumerator MeteorRainCoroutine()
    {
        yield return new WaitForSeconds(meteorRainCooldown);

        for (int i = 0; i < numberOfMeteors; i++)
        {
            // Calculate meteor spwn position above the player's position
            Vector3 meteorSpawnPos = new Vector3(player.position.x, player.position.y + 20f, player.position.z);

            // Instantiate meteor at calculated position
            GameObject newMeteor = Instantiate(meteorPrefab, meteorSpawnPos, Quaternion.identity);
            EnemyProjectileCollision projectileCollision = newMeteor.GetComponent<EnemyProjectileCollision>();
            projectileCollision.projectileDamage = meteorDamage;

            if (MeteorSpawnClips.Length > 0)
            {
                AudioClip randomClip = MeteorSpawnClips[Random.Range(0, MeteorSpawnClips.Length)];
                audioSource.clip = randomClip;
                audioSource.Play(); // This plays the assigned audio clip
            }

            yield return new WaitForSeconds(timeBetweenMeteors);
        }

        isMeteorRaining = false;
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

            // Instantiate the damage popup prefab
            GameObject damagePopup = Instantiate(DamagePopupPrefab, transform.position, Quaternion.identity);
            DamagePopupController popupController = damagePopup.GetComponent<DamagePopupController>();

            // Set the damage value to be displayed in the popup
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
                }

                Invoke(nameof(DestroyEnemy), 1.5f);

            }
        }

    }

    private void DestroyEnemy()
    {
        // Destroy the enemy
        Destroy(gameObject);

        //put the scene
        SceneManager.LoadScene(4);
    }


    private void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        Gizmos.DrawWireSphere(shootingPoint.position, attackRange); // Display AOE damage area
        Gizmos.color = Color.yellow;
        Gizmos.DrawWireSphere(transform.position, sightRange);
    }
}