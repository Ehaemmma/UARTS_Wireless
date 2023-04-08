// src/index.js

import React, { useEffect, useState } from 'react';
import io from 'socket.io-client';
import './App.css';

const socket = io();

function App() {
  const [message, setMessage] = useState('');
  // const activeCard = (cardID) => {
  //   setStyle({display: 'block'});
  // }

  useEffect(() => {
    socket.on('data', data => {
      setMessage(data.message);

    });

    return () => socket.disconnect();
  }, []);

  return (
    <div id="container">
        <img id="card1" class="card" src="gif/a.gif" alt="A"></img>
        <img id="card2" class="card" src="gif/b.gif" alt="B"></img>
        <img id="card3" class="card" src="gif/c.gif" alt="C"></img>
        <h1>{message}</h1>
    </div>
  );
}

export default App; 
