import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const GamesList = () => {
    const { jwtToken, isAdmin } = useContext(AuthContext);
    const [games, setGames] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');
    const [page, setPage] = useState(0);
    const [totalPages, setTotalPages] = useState(0);
    const [filters, setFilters] = useState({ name: '', yearOfRelease: '', creator: '' });
    const navigate = useNavigate();

    const fetchGames = async () => {
        setLoading(true);
        setError('');
        try {
            const params = {
                page: page,
                size: 9, // Adjust items per page as needed
                ...filters
            };
            const data = await api.getAllGames(jwtToken, params);
            setGames(data.content);
            setTotalPages(data.totalPages);
        } catch (err) {
            setError(err.message || 'Failed to fetch games.');
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        fetchGames();
    }, [page, filters]); // Re-fetch when page or filters change

    const handleFilterChange = (e) => {
        setFilters({ ...filters, [e.target.name]: e.target.value });
        setPage(0); // Reset to first page on filter change
    };

    const handleDelete = async (id) => {
        if (window.confirm('Are you sure you want to delete this game?')) {
            try {
                await api.deleteGame(jwtToken, id);
                fetchGames(); // Refresh list
            } catch (err) {
                setError(err.message || 'Failed to delete game.');
            }
        }
    };

    if (loading) return <div className="text-center p-8">Loading games...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8">
            <h2 className="text-3xl font-bold mb-6 text-center">Games List</h2>

            {/* Filter Section */}
            <div className="bg-white p-6 rounded-lg shadow-md mb-8">
                <h3 className="text-xl font-semibold mb-4">Filter Games</h3>
                <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
                    <div>
                        <label htmlFor="name" className="block text-gray-700 text-sm font-bold mb-2">Name</label>
                        <input
                            type="text"
                            id="name"
                            name="name"
                            value={filters.name}
                            onChange={handleFilterChange}
                            className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        />
                    </div>
                    <div>
                        <label htmlFor="yearOfRelease" className="block text-gray-700 text-sm font-bold mb-2">Year of Release</label>
                        <input
                            type="number"
                            id="yearOfRelease"
                            name="yearOfRelease"
                            value={filters.yearOfRelease}
                            onChange={handleFilterChange}
                            className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        />
                    </div>
                    <div>
                        <label htmlFor="creator" className="block text-gray-700 text-sm font-bold mb-2">Creator</label>
                        <input
                            type="text"
                            id="creator"
                            name="creator"
                            value={filters.creator}
                            onChange={handleFilterChange}
                            className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        />
                    </div>
                </div>
            </div>

            {isAdmin && (
                <div className="mb-6 text-right">
                    <Link
                        to="/games/new"
                        className="bg-green-500 hover:bg-green-700 text-white font-bold py-2 px-4 rounded-full transition-colors"
                    >
                        Add New Game
                    </Link>
                </div>
            )}

            {games.length === 0 ? (
                <p className="text-center text-gray-600">No games found.</p>
            ) : (
                <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6">
                    {games.map((game) => (
                        <div key={game.id} className="bg-white rounded-lg shadow-md overflow-hidden transform hover:scale-105 transition-transform duration-300">
                            <img
                                src={`https://placehold.co/400x200/4F46E5/FFFFFF?text=${game.name.substring(0,10)}`}
                                alt={game.name}
                                className="w-full h-48 object-cover"
                                onError={(e) => { e.target.onerror = null; e.target.src="https://placehold.co/400x200/cccccc/000000?text=Game+Image"; }}
                            />
                            <div className="p-5">
                                <h3 className="text-xl font-semibold mb-2 text-blue-800">{game.name}</h3>
                                <p className="text-gray-600 text-sm">Year: {game.yearOfRelease}</p>
                                <p className="text-gray-600 text-sm mb-4">Creator: {game.creator}</p>
                                <Link
                                    to={`/games/${game.id}`}
                                    className="bg-blue-500 hover:bg-blue-700 text-white text-sm font-bold py-2 px-4 rounded-full mr-2 transition-colors"
                                >
                                    View Details
                                </Link>
                                {isAdmin && (
                                    <>
                                        <Link
                                            to={`/games/edit/${game.id}`}
                                            className="bg-yellow-500 hover:bg-yellow-700 text-white text-sm font-bold py-2 px-4 rounded-full mr-2 transition-colors"
                                        >
                                            Edit
                                        </Link>
                                        <button
                                            onClick={() => handleDelete(game.id)}
                                            className="bg-red-500 hover:bg-red-700 text-white text-sm font-bold py-2 px-4 rounded-full transition-colors"
                                        >
                                            Delete
                                        </button>
                                    </>
                                )}
                            </div>
                        </div>
                    ))}
                </div>
            )}

            {/* Pagination Controls */}
            {totalPages > 1 && (
                <div className="flex justify-center mt-8 space-x-4">
                    <button
                        onClick={() => setPage(prev => Math.max(prev - 1, 0))}
                        disabled={page === 0}
                        className="bg-gray-300 hover:bg-gray-400 text-gray-800 font-bold py-2 px-4 rounded-full disabled:opacity-50 transition-colors"
                    >
                        Previous
                    </button>
                    <span className="self-center text-gray-700">Page {page + 1} of {totalPages}</span>
                    <button
                        onClick={() => setPage(prev => Math.min(prev + 1, totalPages - 1))}
                        disabled={page === totalPages - 1}
                        className="bg-gray-300 hover:bg-gray-400 text-gray-800 font-bold py-2 px-4 rounded-full disabled:opacity-50 transition-colors"
                    >
                        Next
                    </button>
                </div>
            )}
        </div>
    );
};

export default GamesList;