---
title: "test bluetooth"
---
<button id="connect-button">Test bluetooth</button>
<script>
const deviceUUID = '6e400001-b5a3-f393-e0a9-e50e24dcca9e'; // Replace with the UUID of your device
let connectedDevice = null;
async function searchForDevices() {
  try {
    const device = await navigator.bluetooth.requestDevice({
      filters: [{ services: [deviceUUID] }],
    });
    // Create a new list item to display the device name
    const listItem = document.createElement('li');
    listItem.textContent = device.name;
    listItem.dataset.device = device;
    listItem.addEventListener('click', handleDeviceSelection);
    document.getElementById('device-list').appendChild(listItem);
  } catch (error) {
    console.error(error);
  }
}
async function connectToDevice(device) {
  try {
    const server = await device.gatt.connect();
    const service = await server.getPrimaryService(deviceUUID);
    const characteristic = await service.getCharacteristic(deviceUUID);
    await characteristic.writeValue(new Uint8Array([1, 2, 3])); // Replace with the numbers you want to send
    console.log('Datasent successfully');
    // Update the connectedDevice variable
    connectedDevice = device;
    // Update the connection status indicator
    const statusIndicator = document.getElementById('connection-status');
    statusIndicator.textContent = 'Connected';
  } catch (error) {
    console.error(error);
  }
}
function handleDeviceSelection(event) {
  const device = event.target.dataset.device;
  connectToDevice(device);
}
// Add an event listener to the "Disconnect" button
document.getElementById('disconnect-button').addEventListener('click', () => {
  if (connectedDevice) {
    connectedDevice.gatt.disconnect();
    // Update the connectedDevice variable and the connection status indicator
    connectedDevice = null;
    const statusIndicator = document.getElementById('connection-status');
    statusIndicator.textContent = 'Disconnected';
  }
});
</script>