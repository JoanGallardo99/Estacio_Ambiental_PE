import requests
import time
import random

# --- CONFIGURACIÓ SENTILO ---
baseUrl = "http://147.83.83.21:8081/data/grup_3-102@provider"
IDENTITY_KEY = "61b39a9081e1ab55fedb391e10778cadcd8cedcbf98810e91ce6aa2ce70925f9"

# --- FUNCIÓ PER ENVIAR (EQUIVALENT A enviarASentilo()) ---
def enviarASentilo(component, valor):
    url = f"{baseUrl}/{component}/{valor}"
    headers = {
        "IDENTITY_KEY": IDENTITY_KEY
    }

    try:
        r = requests.put(url, headers=headers, data="")
        print(f"[{component}] -> HTTP {r.status_code}")
        if r.text:
            print("Resposta:", r.text)
    except Exception as e:
        print(f"Error enviant {component}: {e}")

# --- TEST MULTI-SENSOR ---
if __name__ == "__main__":
    print("Iniciant TEST multi-sensor...")

    # Simulem valors com els del teu TCS34725
    temp = random.randint(-20, 50)
    hum = random.randint(0,100)
    light = random.randint(1000, 5000)
    sound = random.randint(0, 120)

    # Enviar dades exactament amb la mateixa estructura
    enviarASentilo("temperature", temp)
    time.sleep(0.1)
    enviarASentilo("humidity", hum)
    time.sleep(0.1)
    enviarASentilo("lighting", light)
    time.sleep(0.1)
    enviarASentilo("sound", sound)
    time.sleep(0.1)

    print("\nTest finalitzat.")
