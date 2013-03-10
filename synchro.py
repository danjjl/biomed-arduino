#!/usr/bin/python
# -*- coding: utf-8 -*-
import serial
import time
from PyQt4 import QtSql

#Utilisé pour lire des données
def readValue(ser):
    received = False
    while not received:
        try:
            value = ser.read()
        except SerialException:
            received = False
        else:
            received = True
            ser.write('s'.encode("ascii"))
            time.sleep(0.1)
    return value

#Utilisé pour envoyer des données
def sendValue(ser, send):
    sent = False
    while not sent:
        try:
            ser.write(send.encode("ascii"))
            value = ser.read()
        except SerialException:
            sent = False
        else:
            sent = True
            time.sleep(0.1)

def cleanMesures(mesures):
    while mesures.__contains__(''):
        mesures.remove('')
    while mesures.__contains__('0'):
        mesures.remove('0')
    return mesures


def synchro():
    ser = serial.Serial('/dev/ttyUSB1', 9600, timeout = 0.5) #Initialize connection (FAUT MANUELLEMENT CHANGER LE NOM DU PORT)(CA VA PAS DANS LE PROGRAMME FINAL!)
    """Collecte données"""
    value = 'g'
    mesures = []
    sendValue(ser, 's')#Start connection
    while value != '0':
        value = readValue(ser)
        mesures.append(value)

        print value #TODO remove release DEBUG

    mesures = cleanMesures(mesures) #Enlève 0 et mesures vides

    print 'Collected mesures' #TODO remove release DEBUG

    """Sauve données dans db"""
    query = QtSql.QSqlQuery()
    for i in range(0, len(mesures)/10):

        """Transforme un nb <10 en espace+digit"""
        if len(mesures[(i*10)+6]) == 1:
            mesures[(i*10)+7] = "0" + mesures[(i*10)+6]
        if len(mesures[(i*10)+7]) == 1:
            mesures[(i*10)+6] = "0" + mesures[(i*10)+6]

        query.exec_("INSERT INTO mesures (utilisateur, poids, taille, temperature, frequence, time) mesures("+mesures[(i*10)]+", "+mesures[(i*10) + 2]+", "+mesures[(i*10)+1]+", "+str((float(mesures[(i*10)+3])/10)+ 35)+", "+mesures[(i*10)+4]+", DATETIME('20"+mesures[(i*10)+5]+"-"+mesures[(i*10)+6]+"-"+mesures[(i*10)+7]+" "+mesures[(i*10)+8]+":"+mesures[(i*10)+9]+":01'))")

    """Envois liste des utilisateurs"""
    readValue(ser) #Attend que l'arduino confirme qu'il est pret
    query.exec_("SELECT id, nom, prenom FROM utilisateurs")
    while query.next():

        curId = query.value(0).toInt()[0] #Id utilisateur
        lastName = query.value(1).toString() #Nom
        firstName = query.value(2).toString() #Prénom

        for i in range(len(firstName), 5):
            firstName += " "

        sendValue(ser, str(curId))
        sendValue(ser, str(lastName[0]))
        for i in range(0, 5):
             sendValue(ser, str(firstName[i]))

    sendValue(ser, '0') #Code de fin de transfert

db = QtSql.QSqlDatabase.addDatabase('QSQLITE')
db.setDatabaseName('biomed.sql')
db.open()
synchro()
