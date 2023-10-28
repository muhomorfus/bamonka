<template>
  <main class="container">
      <br>

    <div class="card-header d-flex justify-content-between">
      <div>
        <h1>Employee</h1>
      </div>
      <div>
        <a v-if="isHR || isAdmin" class="btn btn-primary" href="/employee/create">New</a>
      </div>
    </div>

      <div class="row">

        <div v-for="e in employee" :key="e.id" class="col-md-6">
          <div class="card mt-4">
            <div class="card-body">
              <h4>
                {{ e.name }} <span class="text-muted">@{{ e.nickname }}</span>
              </h4>

              <p class="text-muted">
                {{ e.description }}
              </p>

              <a class="card-link" :href="`/employee/${e.nickname}`">View</a>
              <a v-if="isHR || isAdmin" class="card-link" :href="`/employee/${e.nickname}/edit`">Edit</a>
              <a v-if="isHR || isAdmin" href="#" class="card-link" @click.prevent="fireEmployee(e.id)">Fire</a>
              <a v-if="isAdmin" href="#" class="card-link" @click.prevent="deleteEmployee(e.id)">Delete</a>
              </div>
            </div>
          </div>
      </div>

    <h1 class="mt-5" v-if="fired.length">Fired</h1>
    <div class="row">
      <div v-for="e in fired" :key="e.id" class="col-md-6">
        <div class="card mt-4">
          <div class="card-body">
            <h4>
              {{ e.name }} <span class="text-muted">@{{ e.nickname }}</span>
            </h4>

            <p class="text-muted">
              {{ e.description }}
            </p>

            <a class="card-link" :href="`/employee/${e.nickname}`">View</a>
            <a v-if="isAdmin" href="#" class="card-link" @click.prevent="deleteEmployee(e.id)">Delete</a>
          </div>
        </div>
      </div>
    </div>

  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import employee from "@/store/modules/employee";

export default defineComponent({
  methods: {
    ...mapActions(['loadEmployee', 'fireEmployee', 'deleteEmployee']),
  },
  created() {
    this.loadEmployee()
  },
  computed: {
    ...mapGetters(['employee', 'fired', 'isHR', 'isAdmin']),
  },
});
</script>