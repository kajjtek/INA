import React from 'react';
import {Link} from 'react-router-dom';


const Home = () => (
    <div className="min-h-[calc(100vh-80px)] flex flex-col items-center justify-center p-4 bg-gradient-to-br from-blue-50 to-indigo-100">
        <div className="text-center bg-white p-8 md:p-12 rounded-xl shadow-2xl mx-auto transform hover:scale-105 transition-transform duration-500 w-full"> {}
            <h1 className="text-5xl md:text-6xl font-extrabold text-gray-900 mb-6 leading-tight">Welcome to <span className="text-white-600">Game Reviews!</span></h1>
            {}
            <p className="text-xl md:text-2xl text-gray-700 mb-8 mx-auto">
                Discover, rate, and discuss your favorite video games with a vibrant community.
            </p>
            <div className="mt-8">
                <Link to="/games" className="inline-block text-white bg-blue-600 hover:bg-blue-700 font-bold py-3 px-8 rounded-full text-lg shadow-lg transform hover:scale-105 transition-all duration-300">
                    Explore Games
                </Link>
            </div>
        </div>
    </div>
);

export default Home;