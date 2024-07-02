using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GunController : MonoBehaviour
{
    public Slider manaBarSlider;
    public bool isFiring;
    public BulletController bullet;
    public float bulletSpeed;
    public Transform player;
    public bool readyToShoot = true;
    public float fireRate;
    public AudioSource myAudioSource;
    public AudioClip myAudioClip;
    public float playerMana = 100.0f;
    public float maxMana = 100.0f;
    private bool enoughMana;
    private float lastShootTime;
    public float manaRechargeRate; 

    public float timeBetweenShots;
    private float shotCounter;
    private float animDelay = 0.4f;

    public Transform firePoint;

    private void Start()
    {
        player = GameObject.Find("PlayerMesh").transform;
        maxMana = PlayerPrefs.GetFloat("PlayerMaxMana", maxMana);
        UpdateManaBar();
    }

    // Update is called once per frame
    void Update()
    {
        
        if (Input.GetMouseButton(1))
        {
            Invoke(nameof(Shoot), animDelay);
        }
        
        if(playerMana < maxMana && readyToShoot)
        {
            playerMana += manaRechargeRate * Time.deltaTime;
            UpdateManaBar();
        }
        if (manaBarSlider != null)
        {
            manaBarSlider.value = playerMana;
        }
    }

    void Shoot()
    {
        if (playerMana > 10.0f)
        {
            enoughMana = true;
            if (Time.time > lastShootTime + fireRate)
            {
                lastShootTime = Time.time;
                readyToShoot = false;
                BulletController newBullet = Instantiate(bullet, firePoint.position, firePoint.rotation) as BulletController;
                newBullet.speed = bulletSpeed;
                myAudioSource.PlayOneShot(myAudioClip);
                Invoke(nameof(ResetShot), fireRate);
                playerMana -= bullet.manaCost;
                UpdateManaBar();
                Debug.Log(playerMana);
            }
        }
    }
    private void ResetShot()
    {
        readyToShoot = true;
    }
    void UpdateManaBar()
    {
        if (manaBarSlider != null)
        {
            // Calculate the ratio of current mana to max mana
            float manaRatio = playerMana / maxMana;

            // Update the mana bar slider's max value and current value
            manaBarSlider.maxValue = maxMana;
            manaBarSlider.value = playerMana;
        }
    }
}
