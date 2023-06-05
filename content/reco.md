---
title: mode reconnaissance
---
<!DOCTYPE html>
<html>
<body>
    <div id="centralwidget">
        <button class="button" id="pushButton">commencer</button>
        <button class="button" id="pushButton_2">arrêter</button>
        <button class="button" id="pushButton_5">enregistrer</button>
        <button class="button" id="pushButton_6">charger</button>
        <button class="button" id="pushButton_7">retour</button>
        <button class="button" id="pushButton_8">quitter</button>
        <textarea id="textEdit"></textarea>
        <label id="label">mode reconnaissance</label>
    </div>
    <script>
        // Récupération des éléments HTML
const pushButton = document.getElementById("pushButton");
const pushButton2 = document.getElementById("pushButton_2");
const pushButton5 = document.getElementById("pushButton_5");
const pushButton6 = document.getElementById("pushButton_6");
const pushButton7 = document.getElementById("pushButton_7");
const pushButton8 = document.getElementById("pushButton_8");
const textEdit = document.getElementById("textEdit");
const label = document.getElementById("label");
// Fonction pour le bouton "commencer"
pushButton.addEventListener("click", function() {
    label.innerHTML = "Mode reconnaissance en cours...";
});
// Fonction pour le bouton "arrêter"
pushButton2.addEventListener("click", function() {
    label.innerHTML = "Mode reconnaissance arrêté.";
});
// Fonction pour le bouton "enregistrer"
pushButton5.addEventListener("click", async function() {
  const text = textEdit.value;
  const fileNameInput = document.createElement("input");
  fileNameInput.type = "text";
  fileNameInput.value = "mon_fichier.txt";
  const fileName = fileNameInput.value;
  const blob = new Blob([text], { type: "text/plain;charset=utf-8" });
  const options = {
    suggestedName: fileName,
    types: [{
      description: "Fichiers texte",
      accept: {
        "text/plain": [".txt"]
      }
    }]
  };
  try {
    const handle = await window.showSaveFilePicker(options);
    const writable = await handle.createWritable();
    await writable.write(blob);
    await writable.close();
  } catch (err) {
    console.error(err);
  }
});
// Fonction pour le bouton "charger"
pushButton6.addEventListener("click", function() {
    const input = document.createElement("input");
    input.type = "file";
    input.accept = "text/plain";
    input.onchange = function(event) {
        const file = event.target.files[0];
        const reader = new FileReader();
        reader.onload = function(event) {
            const text = event.target.result;
            textEdit.value = text;
            alert("Chargement terminé !");
        };
        reader.readAsText(file);
    };
    input.click();
});
// Fonction pour le bouton "quitter"
pushButton8.addEventListener("click", function() {
    if(confirm("Êtes-vous sûr de vouloir quitter ?")) {
        window.close();
    }
});
// Fonction pour le bouton "retour"
pushButton7.addEventListener("click", function() {
    history.back();
});
    </script>
</body>
</html>