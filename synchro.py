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
            value = ser.readline()
            #value = ord(value)
        except serial.SerialException:
            received = False
        else:
            received = True
            ser.write('_'.encode("ascii"))
            time.sleep(0.1)
    return value

#Utilisé pour envoyer des données
def sendValue(ser, send, stop):
    sent = False
    while not sent:
        try:
            ser.write(send.encode("ascii"))
            value = ser.read()
        except serial.SerialException:
            sent = False
        else:
            if value == stop:
                sent = True
                time.sleep(0.1)

def cleanMesures(mesures):
    while mesures.__contains__(''):
        mesures.remove('')
    while mesures.__contains__('0'):
        mesures.remove('0')
    return mesures


def synchro():
    ser = serial.Serial('/dev/ttyUSB2', 9600, timeout = 0.5) #Initialize connection (FAUT MANUELLEMENT CHANGER LE NOM DU PORT)(CA VA PAS DANS LE PROGRAMME FINAL!)
    """Collecte données"""
    value = 'g'
    mesures = []
    sendValue(ser, 's', 'r')#Start connection
    while value != '0':
        value = readValue(ser)
        mesures.append(value)

        print value #TODO remove release DEBUG

    mesures = cleanMesures(mesures) #Enlève 0 et mesures vides
    print mesures
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
    while value != 'r': #Attend que l'arduino confirme qu'il est pret
        value = readValue(ser)

    query.exec_("SELECT id, nom, prenom FROM utilisateurs")
    while query.next():

        curId = query.value(0).toInt()[0] #Id utilisateur
        lastName = query.value(1).toString() #Nom
        firstName = query.value(2).toString() #Prénom

        for i in range(len(firstName), 6):
            firstName += " "

        sendValue(ser, str(curId), 'o')
        sendValue(ser, str(lastName[0]), 'o')
        for i in range(0, 6):
             sendValue(ser, str(firstName[i]), 'o')

    sendValue(ser, '0', 'f') #Code de fin de transfert

db = QtSql.QSqlDatabase.addDatabase('QSQLITE')
db.setDatabaseName('biomed.sql')
db.open()
synchro()
