# Build3
Dit is mijn code voor een bosbrandpreventie systeem.

Deze repository bevat enkele files waaronder: Een code en Libraries.

De code werkt als volgt.
De code gaat eerst via WiFi een connectie aanleggen via MQTT en Nodered tussen de Raspberry Pi en de ESP32-S3.
Als deze connectie gelukt is gaat de temperatuur sensor De temperatuur waardes opnemen uit de omgeving. De sensor is aangesloten op de ESP32-S3 via een digitale poort. Daar moet je zeker rekening mee houden, want anders gaat de sensor geen waardes doorgeven.
Zodra dat de waardes die de sensor doorgeeft over de aangegeven limiet komen, dan gaat de code een melding genereren die wordt verstuurd via Telegram.
Bij de melding die Telegram verstuurd wordt ook de temperatuur weergegeven die er voor zorgt dat de limiet wordt overschreden.

Een aantal tests die je kunt uitvoeren bij deze code:
1: 
2: 
3: 
4:
