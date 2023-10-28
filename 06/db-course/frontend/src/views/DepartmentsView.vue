<template>
  <main class="container">
      <br>

    <div class="card-header d-flex justify-content-between">
      <div>
        <h1>Departments</h1>
      </div>
      <div>
        <a v-if="isAdmin" class="btn btn-primary" href="/departments/create">New</a>
      </div>
    </div>

      <div class="row">

        <div v-for="d in departments" :key="d.id" class="col-md-6">
          <div class="card mt-4">
            <div class="card-body">
              <h4>
                {{ d.name }}
              </h4>

              <p class="text-muted">
                {{ d.description }}
              </p>

              <a class="card-link" :href="`/departments/${d.id}`">View</a>
              <a v-if="isAdmin" class="card-link" :href="`/departments/${d.id}/edit`">Edit</a>
              <a v-if="isAdmin" href="#" class="card-link" @click.prevent="deleteDepartment(d.id)">Delete</a>
              </div>
            </div>
          </div>
      </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";

export default defineComponent({
  methods: {
    ...mapActions(['loadDepartments', 'deleteDepartment']),
  },
  created() {
    this.loadDepartments()
  },
  computed: {
    ...mapGetters(['departments', 'isAdmin']),
  },
});
</script>