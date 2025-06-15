import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

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

    if (loading && isEditing) return <div className="text-center p-8">Loading game data...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8 max-w-md">
            <h2 className="text-3xl font-bold mb-6 text-center">{isEditing ? 'Edit Game' : 'Add New Game'}</h2>
            <form onSubmit={handleSubmit} className="bg-white p-8 rounded-lg shadow-lg">
                <div className="mb-4">
                    <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="gameName">
                        Name
                    </label>
                    <input
                        type="text"
                        id="gameName"
                        name="name"
                        value={game.name}
                        onChange={handleChange}
                        className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        required
                    />
                </div>
                <div className="mb-4">
                    <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="yearOfRelease">
                        Year of Release
                    </label>
                    <input
                        type="number"
                        id="yearOfRelease"
                        name="yearOfRelease"
                        value={game.yearOfRelease}
                        onChange={handleChange}
                        className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        required
                    />
                </div>
                <div className="mb-6">
                    <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="creator">
                        Creator
                    </label>
                    <input
                        type="text"
                        id="creator"
                        name="creator"
                        value={game.creator}
                        onChange={handleChange}
                        className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        required
                    />
                </div>
                <div className="flex items-center justify-between">
                    <button
                        type="submit"
                        className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded-full focus:outline-none focus:shadow-outline transition-colors"
                        disabled={loading}
                    >
                        {loading ? 'Saving...' : (isEditing ? 'Update Game' : 'Add Game')}
                    </button>
                    <button
                        type="button"
                        onClick={() => navigate('/games')}
                        className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-2 px-4 rounded-full focus:outline-none focus:shadow-outline transition-colors"
                    >
                        Cancel
                    </button>
                </div>
            </form>
        </div>
    );
};

export default GameForm;