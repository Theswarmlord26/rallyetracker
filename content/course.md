---
title: mode course
---
<head>
	<title>Lecture de fichier texte</title>
	<meta charset="utf-8">
	<style type="text/css">
		#text {
			height: 20em; /* Hauteur de la zone de texte */
			overflow: auto; /* Défilement automatique */
			border: 1px solid black; /* Bordure */
			padding: 5px; /* Espacement interne */
			font-size: 1.2em; /* Taille de police */
			font-family: monospace; /* Police de caractères */
		}
	</style>
</head>
<body>
	<h1>Lecture de fichier texte</h1>
	<p>Cliquez sur le bouton ci-dessous pour ouvrir un fichier texte :</p>
	<input type="file" id="file" name="file">
	<br><br>
	<button onclick="loadText()">Charger le fichier</button>
	<br><br>
	<div id="text"></div>
	<script>
		var fileInput = document.getElementById('file');
		var textDiv = document.getElementById('text');
		var textArray = []; // Tableau contenant toutes les lignes du fichier texte
		// Fonction appelée à chaque nouvel événement Bluetooth
		function onNewBluetoothEvent(data) {
			// Ajouter la nouvelle ligne à la fin du tableau
			textArray.push(data);
			// Mettre à jour le contenu de la zone de texte
			textDiv.innerHTML = textArray.join("<br>"); // Joindre les lignes avec des sauts de ligne HTML
			// Faire défiler automatiquement la zone de texte jusqu'à la dernière ligne
			textDiv.scrollTop = textDiv.scrollHeight;
		}
		// Fonction appelée lors du clic sur le bouton "Charger le fichier"
		function loadText() {
			var file = fileInput.files[0];
			if (file) {
				var reader = new FileReader();
				reader.readAsText(file);
				reader.onload = function() {
					// Récupérer toutes les lignes du fichier texte dans un tableau
					textArray = reader.result.split("\n");
					// Mettre à jour le contenu de la zone de texte
					textDiv.innerHTML = textArray.join("<br>"); // Joindre les lignes avec des sauts de ligne HTML
					// Faire défiler automatiquement la zone de texte jusqu'à la première ligne
					textDiv.scrollTop = 0;
				}
			}
		}
	</script>
</body>