import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';

// Import Auth Context and Protected Route
import { AuthProvider } from './context/AuthContext';
import ProtectedRoute from './components/common/ProtectedRoute.jsx'; // default export

// Import your page components
import Home from './components/Home.jsx';
import Login from './components/auth/Login';
import Register from './components/auth/Register';
import Navbar from './components/common/Navbar';
import GamesList from './components/games/GamesList';
import GameDetail from './components/games/GameDetail';
import GameForm from './components/games/GameForm';
import ReviewsList from './components/reviews/ReviewsList';
import ReviewDetail from './components/reviews/ReviewDetail';
import ReviewForm from './components/reviews/ReviewForm';
import AdminPanel from './components/admin/AdminPanel';

export default function App() {
    return (
        <AuthProvider>
            <Router>
                <Navbar />
                <main className="py-8">
                    <Routes>
                        <Route path="/" element={<Home />} />
                        <Route path="/login" element={<Login />} />
                        <Route path="/register" element={<Register />} />

                        {/* Protected Routes */}
                        <Route path="/games" element={<ProtectedRoute><GamesList /></ProtectedRoute>} />
                        <Route path="/games/:id" element={<ProtectedRoute><GameDetail /></ProtectedRoute>} />
                        <Route path="/games/new" element={<ProtectedRoute adminOnly={true}><GameForm /></ProtectedRoute>} />
                        <Route path="/games/edit/:id" element={<ProtectedRoute adminOnly={true}><GameForm /></ProtectedRoute>} />

                        <Route path="/reviews" element={<ProtectedRoute><ReviewsList /></ProtectedRoute>} />
                        <Route path="/reviews/:id" element={<ProtectedRoute><ReviewDetail /></ProtectedRoute>} />
                        <Route path="/reviews/new" element={<ProtectedRoute><ReviewForm /></ProtectedRoute>} />
                        <Route path="/reviews/edit/:id" element={<ProtectedRoute><ReviewForm /></ProtectedRoute>} />


                        {/* Admin Protected Routes */}
                        <Route path="/admin" element={<ProtectedRoute adminOnly={true}><AdminPanel /></ProtectedRoute>} />

                        {/* Catch all other routes */}
                        <Route path="*" element={<NotFound />} />
                    </Routes>
                </main>
            </Router>
        </AuthProvider>
    );
}

const NotFound = () => (
    <div className="text-center p-8 text-gray-600">
        <h1 className="text-4xl font-bold mb-4">404 - Page Not Found</h1>
        <p>Sorry, the page you are looking for does not exist.</p>
        <Link to="/" className="text-blue-500 hover:underline mt-4 block">Go to Home</Link>
    </div>
);