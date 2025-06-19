import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const Navbar = () => {
    const { isAuthenticated, isAdmin, logout } = useContext(AuthContext);
    const navigate = useNavigate();

    const handleLogout = () => {
        logout();
        navigate('/login');
    };

    return (
        <nav className="bg-gray-800 p-4 text-white shadow-md">
            <div className="container mx-auto flex justify-between items-center">
                <Link to="/" className="text-2xl font-bold text-blue-300 hover:text-blue-200">
                    Game Reviews
                </Link>
                <div className="flex space-x-4">
                    <Link to="/" className="hover:text-gray-300 transition-colors">Home</Link>
                    {isAuthenticated && (
                        <>
                            <Link to="/games" className="hover:text-gray-300 transition-colors">Games</Link>
                            {/* Add a placeholder for reviews, maybe "My Reviews" or "All Reviews" */}
                            <Link to="/reviews" className="hover:text-gray-300 transition-colors">Reviews</Link>
                            {isAdmin && (
                                <Link to="/admin" className="hover:text-gray-300 transition-colors">Admin Panel</Link>
                            )}
                            <button onClick={handleLogout} className="text-red-400 hover:text-red-300 transition-colors">Logout</button>
                        </>
                    )}
                    {!isAuthenticated && (
                        <>
                            <Link to="/login" className="hover:text-gray-300 transition-colors">Login</Link>
                            <Link to="/register" className="hover:text-gray-300 transition-colors">Register</Link>
                        </>
                    )}
                </div>
            </div>
        </nav>
    );
};

export default Navbar;