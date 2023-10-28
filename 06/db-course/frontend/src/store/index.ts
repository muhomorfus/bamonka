import Vuex from 'vuex';

import auth from './modules/auth';
import errors from './modules/errors';
import search from './modules/search';
import employer from './modules/employer';
import employee from './modules/employee';
import teams from './modules/teams';
import team from './modules/team';
import departments from './modules/departments';
import department from './modules/department';
import vacations from './modules/vacations';

export default new Vuex.Store({
    modules: {
        auth,
        errors,
        search,
        employer,
        employee,
        teams,
        team,
        department,
        departments,
        vacations,
    },
});