# Rapport pour le TP2

## Author :

ABOELNAGA Ahmed


## Présentation

Ce TP avait pour but d'implementer un service de requêtes/réponses à un serveur DNS en C.

## How To

#### Makefile

**Pour Tout compiler**

```bash
make all
```

**Pour tester la commande** ( avec facebook.com )

```bash
make test
```

**Pour compiler le serveur dns**

```bash
make dns
```

**Pour clean le dossier**

```bash
make clean
```

#### Lancer la commande principale dans le terminal

La commande comporte 3 arguments et une option, une fois compilé se mettre dans le dossier :

```bash
./dns < Nom du domaine > < 4|6 > < DNS server >;
```

**Exemple :**

```bash
./dns www.facebook.com 4 @1.1.1.1
```

Resultat attendu :


```bash
DNS_SERVER TO QUERY : 1.1.1.1
 creation du socket en mode DGRAM (UDP) ... [ok]
 envoi du message ... [ok]
 longueur du message envoye : 34
 reception du message ... [ok]
 longueur du message recu : 79

;; Header flags : rd  qr  ra 
; nb of queries : 1
; nb of answers : 2
; Authorities : 0
; Additionls : 0
=============================================================

============================ Send Query  : 1 ===================
Search For : www.facebook.com
;; Type : 1 	 (AA)    Class : 1 	 (IN) 

============================ Got Answer  : 1 ===================
For :  www.facebook.com

;; Type : 5 	 (Name for an alias)   Class : 1 	 (IN) 
;; Time to Live : 3591 Sec 
;; Canonical Name : star-mini.c10r.facebook.com


============================ Got Answer  : 2 ===================
For :  star-mini.c10r.facebook.com

;; Type : 1 	 (AA)    Class : 1 	 (IN) 
;; Time to Live : 51 Sec 
;; IP : 69.171.250.35		IN		AA


                         .
                         .
                         .
                         .
```

---

```bash
./dns www.facebook.com 6 m.root-servers.net
```

Resultat attendu :


```bash
Resolving DNS Server Name ...
DNS_SERVER TO QUERY : 9.9.9.9
 creation du socket en mode DGRAM (UDP) ... [ok]
 envoi du message ... [ok]
 longueur du message envoye : 36
 reception du message ... [ok]
 longueur du message recu : 52

;; Header flags : rd  qr  ra 
; nb of queries : 1
; nb of answers : 1
; Authorities : 0
; Additionls : 0
=============================================================

============================ Send Query  : 1 ===================
Search For : m.root-servers.net
;; Type : 1 	 (AA)    Class : 1 	 (IN) 

============================ Got Answer  : 1 ===================
For :  m.root-servers.net

;; Type : 1 	 (AA)    Class : 1 	 (IN) 
;; Time to Live : 1908588 Sec 
;; IP : 202.12.27.33		IN		AA

DNS_SERVER TO QUERY : 202.12.27.33
 creation du socket en mode DGRAM (UDP) ... [ok]
 envoi du message ... [ok]
 longueur du message envoye : 34
 reception du message ... [ok]
 longueur du message recu : 494

;; Header flags : rd  qr 
; nb of queries : 1
; nb of answers : 0
; Authorities : 13
; Additionls : 14
=============================================================


                  .
                  .
                  .
                  .
```


# Status :

  Accomplished
