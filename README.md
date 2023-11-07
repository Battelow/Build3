# Build3
Dit is mijn code voor een bosbrandpreventie systeem.

Deze repository bevat enkele files waaronder: Een code en Libraries.

De code werkt als volgt.
De code gaat eerst via WiFi een connectie aanleggen via MQTT en Nodered tussen de Raspberry Pi en de ESP32-S3.
Als deze connectie gelukt is gaat de temperatuur sensor De temperatuur waardes opnemen uit de omgeving. De sensor is aangesloten op de ESP32-S3 via een digitale poort. Daar moet je zeker rekening mee houden, want anders gaat de sensor geen waardes doorgeven.
Zodra dat de waardes die de sensor doorgeeft over de aangegeven limiet komen, dan gaat de code een melding genereren die wordt verstuurd via Telegram.
Bij de melding die Telegram verstuurd wordt ook de temperatuur weergegeven die er voor zorgt dat de limiet wordt overschreden.

Een aantal tests die je kunt uitvoeren bij deze code:
Unit Test:
Om deze test uit te voeren ga je kijken hoe de connectie tussen de ESP32-S3 en de raspberry pi werkt. Om dit te doen kan je op het internet gaan zoeken naar een voorbeeldcode om te kijken hoe dit in zijn werk gaat.

Integratietest:
Bij de integratietest kan je erachter proberen te komen ofdat de temperatuur sensor blijft werken als de raspberry pi uitstaat. 

Stresstest:
Bij deze test kan je kijken of de sensor bij het non stop doorkrijgen van data nog steeds normaal blijft functioneren.

Real-world Tests:
Bij deze test kun je gaan kijken ofdat de sensor op een langere termijn de data blijft opnemen en bij het overschreiden van de limiet een melding gaat sturen en vervolgens dan wachten tot dat de limiet weer word overschreden.

Bronnen:
https://www.instructables.com/Node-Red-Controlled-Web-LED-on-ESP32-With-Raspberr/?fbclid=IwAR0V_E4PJdhXoLEgJ3PNK1NXcOHogV_M0snTiIpv5v-d6uy7GO07LpvmSo0
https://wiki.dfrobot.com/SKU_DFR0975_FireBeetle_2_Board_ESP32_S3#target_0
https://www.dfrobot.com/product-203.html
https://www.dfrobot.com/product-689.html
https://www.raspberrypi.com/products/raspberry-pi-zero/
https://www.kingston.com/en/memory-cards/canvas-select-plus-microsd-card
