import csv, codecs
from datetime import datetime
table = []
fichier = codecs.open("data.txt", encoding="utf8", errors="ignore")

#creation du tableau avec les données brutes
ligne = fichier.readline()
table.append(ligne[:-2]) #suppression des deux caractères cachés à la fin "\r\n"
while ligne:
    ligne = fichier.readline()
    table.append(ligne[:-2])
fichier.close()
#convertion chaine de caractère en isoformat pour datetime
def convertion_date(chaine: str):
    """convertir une date "2024:4:20 16:45"
    en "2024-4-20 16:45"
    """
    new_string = ""
    n = 0
    for c in chaine:
        
        if c == ":" and n !=2 :
            new_string += "-"
            n += 1
        else:
            new_string += c
    return new_string
#creation d'un tableau de dictionnaires de relevés
tab = []
dico={}
for chaine in table :
    if len(chaine) != 0 and chaine[0] == "T":
        dico["temp"] = float(chaine[5:])
    if len(chaine) != 0 and chaine[0] == "H":
        dico["humi"] = float(chaine[5:])
    if len(chaine) != 0 and chaine[0] == "M":
        dico["masse"] = float(chaine[6:-2])
    if len(chaine) != 0 and chaine[0] == "I":
        dico["I"] = float(chaine[4:-2])
    if len(chaine) != 0 and chaine[0] == "2":
        d = convertion_date(chaine)
        dico["date"] = datetime.fromisoformat(d)
    if len(chaine) != 0 and  chaine[0] == "U":    
        dico["U"] = float(chaine[2:-1])
        tab.append(dico)
        dico ={}
    
print(len(tab))  