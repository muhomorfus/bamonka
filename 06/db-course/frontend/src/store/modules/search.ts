import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async search(ctx: any, query: string) {
            try {
                const resp = await API.search(query)

                ctx.commit('saveSearch', resp.data)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },
    },

    mutations: {
        saveSearch(state: any, res: openapi.SearchResponse) {
            state.searchResult = res
        },
    },

    state: {
        searchResult: {} as openapi.SearchResponse,
    },

    getters: {
        searchEmployee(state: any) {
            return state.searchResult.employee
        },

        searchTeams(state: any) {
            return state.searchResult.teams
        },

        searchDepartments(state: any) {
            return state.searchResult.departments
        },
    },
};