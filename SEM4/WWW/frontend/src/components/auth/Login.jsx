import React, { useState, useContext } from 'react'; 
import { Link, useNavigate } from 'react-router-dom';
import AuthContext from '../../context/AuthContext'; 
import { api } from '../../services/api';

const Login = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [error, setError] = useState('');
    const { login } = useContext(AuthContext);
    const navigate = useNavigate();

    const handleSubmit = async (e) => {
        e.preventDefault();
        setError('');
        try {
            const data = await api.login(username, password);
            login(data.token, { id: data.id, username: data.username }, data.roles);
            navigate('/'); 
        } catch (err) {
            setError(err.message || 'Login failed. Please check your credentials.');
        }
    };

    return (
        <div className="container mx-auto p-8 min-h-[calc(100vh-80px)] flex items-center justify-center"> {/* Centrowanie formularza */}
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-2xl max-w-md w-full border border-blue-200"> {/* Ulepszony styl karty formularza */}
                <h2 className="text-4xl font-bold mb-8 text-center text-gray-800">Login</h2>
                <form onSubmit={handleSubmit} className="space-y-6">
                    {error && <p className="text-red-500 bg-red-100 p-3 rounded-md mb-4 text-center border border-red-200">{error}</p>}
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="username">
                            Username
                        </label>
                        <input
                            type="text"
                            id="username"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="password">
                            Password
                        </label>
                        <input
                            type="password"
                            id="password"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 mb-3 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            required
                        />
                    </div>
                    <div className="flex flex-col space-y-4">
                        <button
                            type="submit"
                            className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2.5 px-4 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg"
                        >
                            Sign In
                        </button>
                        <Link to="/register" className="inline-block text-center font-bold text-sm text-blue-600 hover:text-blue-800 transition-colors duration-200">
                            Don't have an account? Register
                        </Link>
                    </div>
                </form>
            </div>
        </div>
    );
};


export default Login;