const { app, BrowserWindow } = require('electron')

app.commandLine.appendSwitch('enable-experimental-web-platform-features')

const createWindow = () => {
  const win = new BrowserWindow({
    width: 1280,
    height: 720,
  })

  win.loadURL('http://localhost:1313/')
  
}

app.whenReady().then(() => {
  createWindow()
  const bleno = require('bleno');
  const fs = require('fs');

  // Définir l'UUID du service Bluetooth
  const serviceUUID = '12345678901234567890123456789012';

  // Définir l'UUID de la caractéristique Bluetooth
  const characteristicUUID = '12345678901234567890123456789012';

  // Définir le nom du fichier de sortie
  const outputFileName = 'data.txt';

  // Définir la fonction de rappel pour recevoir les données Bluetooth
  const onReceiveData = function(data) {
    console.log('Données reçues : ', data);
    
    // Écrire les données dans le fichier de sortie
    fs.appendFile(outputFileName, data, function (err) {
      if (err) throw err;
      console.log('Données écrites dans le fichier.');
    });
  };

  // Définir le service Bluetooth
  const service = new bleno.PrimaryService({
    uuid: serviceUUID,
    characteristics: [
      new bleno.Characteristic({
        uuid: characteristicUUID,
        properties: ['read', 'write', 'notify'],
        onReadRequest: function(offset, callback) {
          console.log('Lecture de la caractéristique demandée.');
          callback(bleno.Characteristic.RESULT_SUCCESS, Buffer.from('Caractéristique de test.'));
        },
        onWriteRequest: function(data, offset, withoutResponse, callback) {
          console.log('Écriture de la caractéristique demandée.');
          onReceiveData(data);
          callback(bleno.Characteristic.RESULT_SUCCESS);
        },
        onSubscribe: function(maxValueSize, updateValueCallback) {
          console.log('Client inscrit à la caractéristique.');
        },
        onUnsubscribe: function() {
          console.log('Client désinscrit de la caractéristique.');
        },
        onNotify: function() {
          console.log('Notification envoyée à tous les clients inscrits.');
        }
      })
    ]
  });

  // Démarrer le serveur Bluetooth
  bleno.on('stateChange', function(state) {
    console.log('État Bluetooth : ', state);
    if (state === 'poweredOn') {
      bleno.startAdvertising('Arduino Nano 33 IoT', [service.uuid], function(err) {
        if (err) throw err;
        console.log('Serveur Bluetooth en cours d\'exécution.');
        bleno.setServices([service], function(err) {
          if (err) throw err;
          console.log('Service Bluetooth défini.');
        });
      });
    } else {
      bleno.stopAdvertising(function(err) {
        if (err) throw err;
        console.log('Serveur Bluetooth arrêté.');
      });
    }
  });

  // Gérer les erreurs
  bleno.on('advertisingStartError', function(err) {
    console.error('Erreur de démarrage de la publicité : ', err);
  });

  bleno.on('servicesSetError', function(err) {
    console.error('Erreur de définition du service : ', err);
  });

  bleno.on('accept', function(clientAddress) {
    console.log('Client connecté : ', clientAddress);
  });

  bleno.on('disconnect', function(clientAddress) {
    console.log('Client déconnecté : ', clientAddress);
  });
})


app.on('window-all-closed', () => {
  app.quit()
})