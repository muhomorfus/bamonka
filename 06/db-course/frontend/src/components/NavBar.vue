<template>
  <header>
    <nav class="navbar navbar-expand-lg navbar-light sticky-top" style="background-color: #ffffff; height: 2em; border-bottom: 1px solid #ced4da">
      <div class="container-fluid">
        <div class="collapse navbar-collapse" id="navbarSupportedContent">
          <ul class="navbar-nav">
            <li class="nav-item active small">
              <a class="nav-link" href="/employee">Employee</a>
            </li>
            <li class="nav-item active small">
              <a class="nav-link" href="/teams">Teams</a>
            </li>
            <li class="nav-item active small">
              <a class="nav-link" href="/departments">Departments</a>
            </li>
            <li class="nav-item active small">
              <a class="nav-link" href="/vacations">Vacations</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>

    <nav class="navbar navbar-expand-lg navbar-light sticky-top" style="background-color: #ffffff; ">
      <div class="container-fluid">
        <div class="collapse navbar-collapse" id="navbarSupportedContent">
          <ul class="navbar-nav">
            <li class="nav-item active">
              <a class="nav-link navbar-brand" href="/">balteca</a>
            </li>
          </ul>
        </div>

        <div class="collapse navbar-collapse">
          <form class="d-flex input-group" @submit.prevent="onSubmit">
            <input class="form-control" type="search" placeholder="Enter you query here..." aria-label="Search" v-model="query">
            <button class="btn btn-outline-primary" type="submit">Search</button>
          </form>
        </div>

        <div class="collapse navbar-collapse justify-content-end" id="navbarSupportedContent">
          <ul class="navbar-nav mr-0">
            <li class="nav-item active">
              <a class="nav-link" :href="`/employee/${me.nickname}`">@{{ me.nickname }}</a>
            </li>
            <li class="nav-item active">
              <a class="nav-link" href="#" @click.prevent="logout">Logout</a>
            </li>
          </ul>
        </div>
      </div>
    </nav>
  </header>
</template>

<style scoped>

</style>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";

export default defineComponent({
  data() {
    return {
      query: '',
    }
  },
  methods: {
    ...mapActions(['logout']),
    onSubmit() {
      if (this.query) {
        window.location.replace('/search?query=' + encodeURIComponent(this.query))
      }
    }
  },
  computed: {
    ...mapGetters(['me'])
  },
})
</script>
