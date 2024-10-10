const { Buffer } = require('node:buffer');


const URL = 'http://0.0.0.0:8080'

const BAD_EMAIL = 'test@invalid.com';
const BAD_PASSWORD = '12345678'


async function sendRequest(email, password) {
    const credentials = email + ':' + password
    const encodedCredentials = Buffer.from(credentials).toString('base64')

    return fetch(URL + '/testAuthMiddleware', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
            'Authorization': 'Basic ' + encodedCredentials
        },
    })
        .then(response => {
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.text()}`);
            }
            return response.json();
        })
        .then(data => {
            return `Success: ${JSON.stringify(data)}`;
        })
        .catch(error => {
            return `Error: ${error.message}`;
        });
}

sendRequest(BAD_EMAIL, BAD_PASSWORD).then(console.log)

