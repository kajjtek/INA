import React, { useContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';
import { api } from '../../services/api';

const GamesList = () => {
    const { jwtToken, isAdmin } = useContext(AuthContext);
    const [games, setGames] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');
    const [page, setPage] = useState(0);
    const [totalPages, setTotalPages] = useState(0);

    const [filterInputs, setFilterInputs] = useState({ name: '', yearOfRelease: '', creator: '' });
    const [filters, setFilters] = useState({ name: '', yearOfRelease: '', creator: '' });

    const navigate = useNavigate();

    const fetchGames = async () => {
        setLoading(true);
        setError('');
        try {
            const params = {
                page: page,
                size: 9,
                ...(filters.name && { name: filters.name }),
                ...(filters.yearOfRelease && { yearOfRelease: filters.yearOfRelease }),
                ...(filters.creator && { creator: filters.creator }),
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
    }, [page, filters, jwtToken]);

    const handleInputChange = (e) => {
        setFilterInputs({ ...filterInputs, [e.target.name]: e.target.value });
    };

    const handleSearchClick = () => {
        setPage(0); 
        setFilters(filterInputs);
    };

    const handleDelete = async (id) => {
        if (window.confirm('Are you sure you want to delete this game?')) {
            try {
                await api.deleteGame(jwtToken, id);
                fetchGames(); 
            } catch (err) {
                setError(err.message || 'Failed to delete game.');
            }
        }
    };

    if (loading) return <div className="text-center p-8 text-xl font-semibold">Loading games...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8">
            <h2 className="text-4xl font-bold mb-8 text-center text-white-800">Games Catalog</h2>

            {/* Filter Section */}
            <div className="bg-white p-6 rounded-xl shadow-lg mb-8 border border-gray-200"> {/* Zaokrąglone rogi i cień */}
                <h3 className="text-2xl font-semibold mb-4 text-gray-800">Filter Games</h3>
                <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-4">
                    <div>
                        <label htmlFor="name" className="block text-gray-700 text-sm font-semibold mb-2">Name</label>
                        <input
                            type="text"
                            id="name"
                            name="name"
                            value={filterInputs.name}
                            onChange={handleInputChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                        />
                    </div>
                    <div>
                        <label htmlFor="yearOfRelease" className="block text-gray-700 text-sm font-semibold mb-2">Year of Release</label>
                        <input
                            type="number"
                            id="yearOfRelease"
                            name="yearOfRelease"
                            value={filterInputs.yearOfRelease}
                            onChange={handleInputChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                        />
                    </div>
                    <div>
                        <label htmlFor="creator" className="block text-gray-700 text-sm font-semibold mb-2">Creator</label>
                        <input
                            type="text"
                            id="creator"
                            name="creator"
                            value={filterInputs.creator}
                            onChange={handleInputChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-blue-500 focus:border-transparent transition duration-200"
                        />
                    </div>
                </div>
                {/* Search Button */}
                <div className="flex justify-end">
                    <button
                        onClick={handleSearchClick}
                        className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md"
                    >
                        Search
                    </button>
                </div>
            </div>

            {isAdmin && (
                <div className="mb-8 text-right"> {/* Zwiększony margines */}
                    <Link
                        to="/games/new"
                        className="bg-green-600 hover:bg-green-700 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md"
                    >
                        Add New Game
                    </Link>
                </div>
            )}

            {games.length === 0 ? (
                <p className="text-center text-gray-600 text-xl">No games found.</p>
            ) : (
                <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-6"> {/* Więcej kolumn na dużych ekranach */}
                    {games.map((game) => (
                        <div key={game.id} className="bg-white rounded-xl shadow-lg overflow-hidden transform hover:scale-105 transition-transform duration-300 border border-gray-200">
                            <div className="p-5">
                                <h3 className="text-2xl font-bold mb-2 text-blue-800 leading-tight">{game.name}</h3> {/* Większy i pogrubiony tytuł */}
                                <p className="text-gray-600 text-base mb-1">Year: {game.yearOfRelease}</p> {/* Normalny rozmiar */}
                                <p className="text-gray-600 text-base mb-4">Creator: {game.creator}</p> {/* Normalny rozmiar */}
                                <div className="flex flex-wrap gap-2 mt-4"> {/* Elastyczne przyciski */}
                                    <Link
                                        to={`/games/${game.id}`}
                                        className="flex-1 text-center bg-blue-500 hover:bg-blue-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                    >
                                        View Details
                                    </Link>
                                    {isAdmin && (
                                        <>
                                            <Link
                                                to={`/games/edit/${game.id}`}
                                                className="flex-1 text-center bg-yellow-500 hover:bg-yellow-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                            >
                                                Edit
                                            </Link>
                                            <button
                                                onClick={() => handleDelete(game.id)}
                                                className="flex-1 text-center bg-red-500 hover:bg-red-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                            >
                                                Delete
                                            </button>
                                        </>
                                    )}
                                </div>
                            </div>
                        </div>
                    ))}
                </div>
            )}

            {/* Pagination Controls */}
            {totalPages > 1 && (
                <div className="flex justify-center mt-10 space-x-4"> {/* Większy margines */}
                    <button
                        onClick={() => setPage(prev => Math.max(prev - 1, 0))}
                        disabled={page === 0}
                        className="bg-gray-300 hover:bg-gray-400 text-gray-800 font-bold py-2.5 px-6 rounded-full disabled:opacity-50 transition-colors duration-200 shadow-md"
                    >
                        Previous
                    </button>
                    <span className="self-center text-gray-700 font-semibold text-lg">Page {page + 1} of {totalPages}</span>
                    <button
                        onClick={() => setPage(prev => Math.min(prev + 1, totalPages - 1))}
                        disabled={page === totalPages - 1}
                        className="bg-gray-300 hover:bg-gray-400 text-gray-800 font-bold py-2.5 px-6 rounded-full disabled:opacity-50 transition-colors duration-200 shadow-md"
                    >
                        Next
                    </button>
                </div>
            )}
        </div>
    );
};

export default GamesList;
