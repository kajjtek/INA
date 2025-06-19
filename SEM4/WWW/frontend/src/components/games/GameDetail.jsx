import React, { useContext,createContext, useState, useEffect} from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate, useParams } from 'react-router-dom';
import { api } from '../../services/api';

const GameDetail = () => {
    const { id } = useParams();
    const { jwtToken, isAdmin } = useContext(AuthContext);
    const [game, setGame] = useState(null);
    const [reviews, setReviews] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');
    const navigate = useNavigate();

    useEffect(() => {
        const fetchGameAndReviews = async () => {
            setLoading(true);
            setError('');
            try {
                const gameData = await api.getGameById(jwtToken, id);
                setGame(gameData);
                const reviewsData = await api.fetchReviewsByGame(jwtToken, id); // Użycie api.fetchReviewsByGame
                setReviews(reviewsData);
            } catch (err) {
                setError(err.message || 'Failed to fetch game details or reviews.');
            } finally {
                setLoading(false);
            }
        };
        fetchGameAndReviews();
    }, [id, jwtToken]);

    const handleDelete = async () => {
        if (window.confirm('Are you sure you want to delete this game?')) {
            try {
                await api.deleteGame(jwtToken, id);
                navigate('/games'); // Redirect after deletion
            } catch (err) {
                setError(err.message || 'Failed to delete game.');
            }
        }
    };

    if (loading) return <div className="text-center p-8 text-xl font-semibold">Loading game details...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;
    if (!game) return <div className="text-center p-8 text-gray-600 text-xl">Game not found.</div>;

    return (
        <div className="container mx-auto p-8">
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-lg mb-8 border border-blue-200"> {/* Ulepszona karta */}
                <h2 className="text-4xl md:text-5xl font-bold mb-4 text-blue-800 leading-tight">{game.name}</h2> {/* Większy tytuł */}
                <p className="text-lg text-gray-700 mb-2"><strong>Year of Release:</strong> {game.yearOfRelease}</p>
                <p className="text-lg text-gray-700 mb-4"><strong>Creator:</strong> {game.creator}</p>

                {isAdmin && (
                    <div className="mt-6 flex flex-wrap gap-3">
                        <Link
                            to={`/games/edit/${game.id}`}
                            className="bg-yellow-500 hover:bg-yellow-600 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md"
                        >
                            Edit Game
                        </Link>
                        <button
                            onClick={handleDelete}
                            className="bg-red-500 hover:bg-red-600 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md"
                        >
                            Delete Game
                        </button>
                    </div>
                )}
            </div>

            <div className="bg-white p-8 md:p-10 rounded-xl shadow-lg border border-gray-200">
                <h3 className="text-3xl font-bold mb-6 text-gray-800">Reviews</h3>
                {reviews.length === 0 ? (
                    <p className="text-gray-600 text-lg">No reviews yet for this game.</p>
                ) : (
                    <div className="space-y-6">
                        {reviews.map(review => (
                            <div key={review.id} className="border-b border-gray-200 pb-4 last:border-b-0">
                                <p className="text-xl font-semibold text-blue-600 mb-1">Rating: {review.rating}</p> {/* Większa ocena */}
                                <p className="text-gray-700 text-base mb-2">{review.description}</p>
                                <p className="text-gray-500 text-sm">By: {review.author ? review.author.username : 'Unknown User'}</p>
                                <Link to={`/reviews/${review.id}`} className="text-blue-500 hover:underline text-sm font-semibold mt-2 inline-block">View Review Details</Link>
                            </div>
                        ))}
                    </div>
                )}
                {/* Option to add a review */}
                <div className="mt-8 text-center">
                    <Link to={`/reviews/new?gameId=${game.id}`} className="bg-blue-600 hover:bg-blue-700 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-md">
                        Add Review
                    </Link>
                </div>
            </div>
        </div>
    );
};


export default GameDetail;