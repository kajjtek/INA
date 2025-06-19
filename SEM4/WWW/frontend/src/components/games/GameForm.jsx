import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate, useParams } from 'react-router-dom';
import { api } from '../../services/api';

const GameForm = () => {
    const { id } = useParams(); // For edit mode
    const { jwtToken } = useContext(AuthContext);
    const navigate = useNavigate();
    const [game, setGame] = useState({ name: '', yearOfRelease: '', creator: '' });
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState('');
    const [isEditing, setIsEditing] = useState(false);

    useEffect(() => {
        if (id) {
            setIsEditing(true);
            const fetchGame = async () => {
                setLoading(true);
                try {
                    const data = await api.getGameById(jwtToken, id);
                    setGame(data);
                } catch (err) {
                    setError(err.message || 'Failed to fetch game for editing.');
                } finally {
                    setLoading(false);
                }
            };
            fetchGame();
        }
    }, [id, jwtToken]);

    const handleChange = (e) => {
        setGame({ ...game, [e.target.name]: e.target.value });
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError('');
        try {
            if (isEditing) {
                await api.updateGame(jwtToken, id, game);
            } else {
                await api.createGame(jwtToken, game);
            }
            navigate('/games'); // Redirect to games list
        } catch (err) {
            setError(err.message || 'Failed to save game.');
        } finally {
            setLoading(false);
        }
    };

    if (loading && isEditing) return <div className="text-center p-8 text-xl font-semibold">Loading game data...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8 min-h-[calc(100vh-80px)] flex items-center justify-center">
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-2xl max-w-md w-full border border-blue-200">
                <h2 className="text-4xl font-bold mb-8 text-center text-gray-800">{isEditing ? 'Edit Game' : 'Add New Game'}</h2>
                <form onSubmit={handleSubmit} className="space-y-6">
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="gameName">
                            Name
                        </label>
                        <input
                            type="text"
                            id="gameName"
                            name="name"
                            value={game.name}
                            onChange={handleChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="yearOfRelease">
                            Year of Release
                        </label>
                        <input
                            type="number"
                            id="yearOfRelease"
                            name="yearOfRelease"
                            value={game.yearOfRelease}
                            onChange={handleChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="creator">
                            Creator
                        </label>
                        <input
                            type="text"
                            id="creator"
                            name="creator"
                            value={game.creator}
                            onChange={handleChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                            required
                        />
                    </div>
                    <div className="flex flex-col sm:flex-row items-center justify-center space-y-4 sm:space-y-0 sm:space-x-4 mt-6">
                        <button
                            type="submit"
                            className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg w-full sm:w-auto"
                            disabled={loading}
                        >
                            {loading ? 'Saving...' : (isEditing ? 'Update Game' : 'Add Game')}
                        </button>
                        <button
                            type="button"
                            onClick={() => navigate('/games')}
                            className="bg-gray-500 hover:bg-gray-600 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg w-full sm:w-auto"
                        >
                            Cancel
                        </button>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default GameForm;