import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';
import { Navigate } from 'react-router-dom'; 

const ProtectedRoute = ({ children, adminOnly = false }) => {
    const { isAuthenticated, isAdmin } = useContext(AuthContext);

    if (!isAuthenticated) {
        // Redirect to login if not authenticated
        return <Navigate to="/login" replace />;
    }

    if (adminOnly && !isAdmin) {
        // Redirect to home or unauthorized page if not admin and adminOnly is true
        return <Navigate to="/" replace />; // Or a specific /unauthorized page
    }

    return children;
};

export default ProtectedRoute;