import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const Home = () => (
    <div className="container mx-auto p-8 text-center">
        <h1 className="text-4xl font-bold mb-4">Welcome to Game Reviews!</h1>
        <p className="text-lg text-gray-700">Discover and review your favorite games.</p>
        {/* <img
            src="https://placehold.co/800x400/ADD8E6/000000?text=Welcome+to+Game+Reviews"
            alt="Welcome Image"
            className="mt-8 rounded-lg shadow-md mx-auto"
            onError={(e) => { e.target.onerror = null; e.target.src="https://placehold.co/800x400/cccccc/000000?text=Image+Unavailable"; }}
        /> */}
    </div>
);

export default Home;