import React, {useState} from 'react';
import AuthContext from '../../context/AuthContext'; 
import { Link, useNavigate } from 'react-router-dom';
import { api } from '../../services/api';

const Register = () => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [description, setDescription] = useState('');
    const [birthDate, setBirthDate] = useState('');
    const [error, setError] = useState('');
    const [success, setSuccess] = useState('');
    const navigate = useNavigate();

    const handleSubmit = async (e) => {
        e.preventDefault();
        setError('');
        setSuccess('');
        try {
            const data = await api.register({ username, password, description, birthDate });
            setSuccess(data);
            setTimeout(() => navigate('/login'), 2000); // Redirect after 2 seconds
        } catch (err) {
            setError(err.message || 'Registration failed.');
        }
    };

    return (
        <div className="container mx-auto p-8 min-h-[calc(100vh-80px)] flex items-center justify-center"> {/* Centrowanie formularza */}
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-2xl max-w-md w-full border border-green-200"> {/* Ulepszony styl karty formularza */}
                <h2 className="text-4xl font-bold mb-8 text-center text-gray-800">Register</h2>
                <form onSubmit={handleSubmit} className="space-y-6">
                    {error && <p className="text-red-500 bg-red-100 p-3 rounded-md mb-4 text-center border border-red-200">{error}</p>}
                    {success && <p className="text-green-500 bg-green-100 p-3 rounded-md mb-4 text-center border border-green-200">{success}</p>}
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="regUsername">
                            Username
                        </label>
                        <input
                            type="text"
                            id="regUsername"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-green-500 focus:border-transparent transition duration-200"
                            value={username}
                            onChange={(e) => setUsername(e.target.value)}
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="regPassword">
                            Password
                        </label>
                        <input
                            type="password"
                            id="regPassword"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 mb-3 leading-tight focus:outline-none focus:ring-2 focus:ring-green-500 focus:border-transparent transition duration-200"
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="regDescription">
                            Description
                        </label>
                        <textarea
                            id="regDescription"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-green-500 focus:border-transparent transition duration-200"
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                        ></textarea>
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="regBirthDate">
                            Birth Date
                        </label>
                        <input
                            type="date"
                            id="regBirthDate"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-green-500 focus:border-transparent transition duration-200"
                            value={birthDate}
                            onChange={(e) => setBirthDate(e.target.value)}
                        />
                    </div>
                    <div className="flex items-center justify-center">
                        <button
                            type="submit"
                            className="bg-green-600 hover:bg-green-700 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg"
                        >
                            Register
                        </button>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default Register;