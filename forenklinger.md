## Forenklinger 
- Der hvor python libraries er brukt trenger vi ikke implementere disse funksjoenen i c++.
- For interpolasjon kan vi bruke en forenklet lineær funksjon i første omgang.

## Generelt
- Vi skal bruke to modules (phys_mod), som er bygget opp av flere num_mod, oppbygningen står forklart i machine.csv.
- Målet med oppgaven er å beregne pressure og temp ved alle punkter i modellen (1D)?
- Pressure beregnes bakover og temp framover. Pressure og temp bestemmer hverandre derfor må man gjøre flere iterasjoner for at verdiene skal stabilisere seg.
- Stopring er overgangen mellom to modules. Her er pressure høyt fordi utgangen er liten. Pressure ut fra en module er lik pressure inn i neste, samme for temp.
- Filling rate er hvor stor del av røret som er fylt. 
