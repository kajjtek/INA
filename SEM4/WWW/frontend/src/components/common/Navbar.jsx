// src/components/common/Navbar.jsx
import React, { useContext } from 'react';
import { Link, useNavigate } from 'react-router-dom';
import AuthContext from '../../context/AuthContext';

const Navbar = () => {
    const { isAuthenticated, isAdmin, logout } = useContext(AuthContext);
    const navigate = useNavigate();

    const handleLogout = () => {
        logout();
        navigate('/login');
    };

    return (
        <nav className="bg-gray-900 p-4 text-white shadow-lg font-inter rounded-b-lg">
            {/* Modified: flex-wrap to allow wrapping on smaller screens, justify-center for overall centering,
                          md:justify-between to separate on medium and larger screens */}
            <div className="container mx-auto flex flex-wrap justify-center items-center md:justify-between space-y-2 md:space-y-0">
                {/* Title: Added text-center to ensure it's centered if it wraps */}
                <Link to="/" className="text-4xl md:text-5xl font-extrabold text-blue-300 hover:text-blue-200 transition-colors duration-300 w-full md:w-auto text-center md:text-left"> {/* Increased size to text-4xl, md:text-5xl for larger screens */}
                    Game Reviews
                </Link>
                {/* Navigation Links: Added flex-grow to push it to the right on md screens and above */}
                <div className="flex flex-wrap justify-center md:justify-end items-center space-x-4 md:space-x-6 text-lg mt-2 md:mt-0 md:flex-grow"> {/* Added md:flex-grow and mt-2/md:mt-0 for spacing */}
                    <Link to="/" className="text-white font-bold hover:text-blue-300 transition-colors duration-300 py-1 px-2 rounded-md">Home</Link>
                    <Link to="/info" className="text-white font-bold hover:text-blue-300 transition-colors duration-300 py-1 px-2 rounded-md">Info</Link>
                    {isAuthenticated && (
                        <>
                            <Link to="/games" className="text-white font-bold hover:text-blue-300 transition-colors duration-300 py-1 px-2 rounded-md">Games</Link>
                            <Link to="/reviews" className="text-white font-bold hover:text-blue-300 transition-colors duration-300 py-1 px-2 rounded-md">Reviews</Link>
                            {isAdmin && (
                                <Link to="/admin" className="text-white font-bold hover:text-blue-300 transition-colors duration-300 py-1 px-2 rounded-md">Admin Panel</Link>
                            )}
                            <button
                                onClick={handleLogout}
                                className="bg-red-600 hover:bg-red-700 text-white font-semibold py-1.5 px-4 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md"
                            >
                                Logout
                            </button>
                        </>
                    )}
                    {!isAuthenticated && (
                        <>
                            <Link to="/login" className="bg-blue-600 hover:bg-blue-700 text-white font-semibold py-1.5 px-4 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md">Login</Link>
                            <Link to="/register" className="bg-green-600 hover:bg-green-700 text-white font-semibold py-1.5 px-4 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md">Register</Link>
                        </>
                    )}
                </div>
            </div>
        </nav>
    );
};

export default Navbar;