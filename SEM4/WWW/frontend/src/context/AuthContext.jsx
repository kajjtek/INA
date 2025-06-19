import React, { createContext, useState, useEffect } from 'react';


const AuthContext = createContext(null);

export const AuthProvider = ({ children }) => {
    const [jwtToken, setJwtToken] = useState(localStorage.getItem('jwtToken'));
    const [currentUser, setCurrentUser] = useState(JSON.parse(localStorage.getItem('currentUser')));
    const [roles, setRoles] = useState(JSON.parse(localStorage.getItem('userRoles')) || []);
    const [isAuthenticated, setIsAuthenticated] = useState(!!localStorage.getItem('jwtToken'));
    const [isAdmin, setIsAdmin] = useState(false);

    useEffect(() => {
        const token = localStorage.getItem('jwtToken');
        const user = JSON.parse(localStorage.getItem('currentUser'));
        const userRoles = JSON.parse(localStorage.getItem('userRoles'));

        setJwtToken(token);
        setCurrentUser(user);
        setRoles(userRoles || []);
        setIsAuthenticated(!!token);
        setIsAdmin(userRoles && userRoles.includes('ROLE_ADMIN'));
    }, []);

    const login = (token, user, userRoles) => {
        localStorage.setItem('jwtToken', token);
        localStorage.setItem('currentUser', JSON.stringify(user));
        localStorage.setItem('userRoles', JSON.stringify(userRoles));
        setJwtToken(token);
        setCurrentUser(user);
        setRoles(userRoles);
        setIsAuthenticated(true);
        setIsAdmin(userRoles.includes('ROLE_ADMIN'));
    };

    const logout = () => {
        localStorage.removeItem('jwtToken');
        localStorage.removeItem('currentUser');
        localStorage.removeItem('userRoles');
        setJwtToken(null);
        setCurrentUser(null);
        setRoles([]);
        setIsAuthenticated(false);
        setIsAdmin(false);
    };

    return (
        <AuthContext.Provider value={{ jwtToken, currentUser, roles, isAuthenticated, isAdmin, login, logout }}>
            {children}
        </AuthContext.Provider>
    );
};

export default AuthContext;